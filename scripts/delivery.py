#!/usr/bin/python3

import glob
import yaml
import os
import pathlib
import fnmatch
import sys
from yamlinclude import YamlIncludeConstructor
import argparse
import shutil


parser = argparse.ArgumentParser()
parser.add_argument("-c", "--config",   help="Delivery config file", type=argparse.FileType("r"), required=True)
parser.add_argument("-t", "--toplevel", help="Delivery top-level directory", required=True)
parser.add_argument("-w", "--workdir",  help="Delivery package location", default=".delivery")
parser.add_argument("-n", "--name",     help="Toplevel archive name", default="toplevel")
args = parser.parse_args()


YamlIncludeConstructor.add_to_loader_class(loader_class=yaml.FullLoader, base_dir='..')
config = yaml.load(args.config, Loader=yaml.FullLoader)

class Delivery():
    always_exclude = [".git"]

    def __init__(self, basepath, workdir, topworkdir, config):
        self.children = []
        self.name = "top"
        self.base = basepath
        self.config = config
        self.workdir = workdir
        self.topworkdir = topworkdir

        if self.config["exclude"] is None:
            self.config["exclude"]=[]

        if not self.config["submodules"] is None:
            for s in self.config["submodules"]:
                path = os.path.join(basepath, s)
                conf = self.config["submodules"][s]
                subworkdir = os.path.join(self.workdir, s)
                sub = Delivery(path, subworkdir, topworkdir, conf)
                sub.name = s
                sub.topworkdir = self.topworkdir
                # excl = s + "/**"
                # self.config["exclude"].append(excl)
                self.children.append(sub)
                # print(f"Adding submodule {s} (excludes {excl})")
                print(f"Adding submodule {s}")

    def reset(self):
        self.files = glob.iglob(self.base + "/**", recursive=True)

    def relpath(self, path):
        return os.path.relpath(path, self.base)

    def check_path_object(self, pobject):
        ret = False
        if os.path.basename(pobject) in self.always_exclude:
            return False

        if "include" in self.config:
            for inc in self.config["include"]:
                if fnmatch.fnmatch(pobject, inc):
                    ret = True

        if "exclude" in self.config:
            for exc in self.config["exclude"]:
                if fnmatch.fnmatch(pobject, exc):
                    ret = False

        return ret

    def handle_file(self, fl):
        src = os.path.join(self.base, self.relpath(fl))
        dst = os.path.join(self.workdir, self.relpath(fl))
        srcdir = pathlib.Path(os.path.dirname(src))
        dstdir = pathlib.Path(os.path.dirname(dst))
        #FixMe: Not directory permissions are handled properly here
        if not dstdir.exists():
            mode = srcdir.stat().st_mode
            os.makedirs(dstdir, mode, exist_ok=True)
        os.link(src,dst)

    def prepare(self):
        self.reset()
        while True:
            try:
                f = next(self.files)
                if not self.check_path_object(self.relpath(f)):
                    continue
                p = pathlib.Path(f)
                if p.is_file():
                    self.handle_file(p)
            except StopIteration:
                break
        for child in self.children:
            child.prepare()

    def archive(self, name = "top", _extra="."):
        cwd = os.getcwd()
        wd = os.path.join(self.topworkdir, ".delivery")
        print(f"Creating delivery file {name}.tgz in {self.topworkdir}")
        os.chdir(wd)
        exclude_opts = []
        for p in self.children:
            exclude_opts.append("--exclude=" + os.path.join(_extra, p.name))
        tarball = os.path.join(self.topworkdir, f"{name}.tgz".split('/')[-1]) # Use split to remove relative path for child when name formed
        cmd = f"tar cpzf {tarball} {' '.join(exclude_opts)} {_extra} --remove-files"
        os.system(cmd)
        os.chdir(cwd)

        for child in self.children:
            child.archive(name = child.name, _extra=os.path.join(_extra,child.name))

    def clean(self):
        wd = os.path.join(self.topworkdir, ".delivery")
        print(f"Cleaning up {wd}")
        shutil.rmtree(wd)

os.makedirs(os.path.join(args.workdir, ".delivery"), exist_ok=True)
wd = os.path.join(args.workdir, ".delivery")
root = Delivery(args.toplevel, wd, args.workdir, config)
root.prepare()
root.archive(name = args.name)
root.clean()
