class CMakeProject {

    def steps
    int tries = 0
    boolean configured = false
    String setupscript
    String sourcedir = ''
    String workdir
    String environment = '''#!/bin/bash -x
    pwd
    '''

    CMakeProject(steps, wdir, srcdir) {
        this.workdir = wdir
        this.setupscript = setupscript
        this.steps = steps
        this.sourcedir = srcdir
        println('Will configure project in: ' + wdir)
    }

    static def getParams(context) {
        return
        [
        ]
    }

    def run(cmd) {
        steps.ws(workdir) {
            steps.sh(environment + cmd)
        }
    }

    def getFilePath(file) {
        return workdir + '/' + file
    }

    def configure(options) {
        def args = ''
        options.each {
            k, v ->
                args += " -D${k}=${v} "
        }
        this.run('cmake' + args + this.sourcedir)
    }

    def build() {
        run('cmake --build .')
    }

    def test(label) {
        run('ctest --no-compress-output -T Test -L ' + label + ' || true')
    }

    def test() {
        run('ctest --no-compress-output -T Test || true')
    }

    def tests(String label) {
        def tests = [:]
        steps.script {
            steps.ws(workdir) {
                def teststmp = steps.sh (
                script: 'ctest -N -L ' + label,
                returnStdout: true
                ).split('\n')
                teststmp.each {
                    line ->
                        def matcher = line =~ /.* Test\s+#(\d+): (.+)/
                        if (matcher.matches()) {
                        int num = matcher[0][1] as int
                        String name =  matcher[0][2] as String
                        tests[name] = num
                            } else {
                        println 'no match : ' + line
                        }
                }
                return tests
            }
        }
    }

    def coverage() {
        run('lcov -c -o coverage.info -d . || true')
    }

    def getCoverageFile() {
        return getFilePath('coverage.info')
    }

    def analyzeResults() {
    }

}

class RumBootProject {

    def build_types = ['Debug', 'Production', 'PostProduction']
    def builds = [:]
    def platform
    def options = [ RUMBOOT_DISABLE_TESTING: 'Yes', RUMBOOT_COVERAGE: 'No' ]
    def steps
    def label = 'full'

    RumBootProject(steps, platform, srcdir) {
        this.platform = platform
        this.steps = steps
        build_types.each {
            tp ->
                def tg = tag(tp)
                builds[tp] = new CMakeProject(steps, srcdir + '/' + tg, srcdir)
                steps.updateGitlabCommitStatus name: tg, state: 'pending'
        }
    }

    @NonCPS def tag(type) {
        return platform + '-' + type
    }

    def dropBuild(type) {
        builds.remove(type)
    }

    def addOptions(opts) {
        this.options += opts
    }

    def enableCoverage() {
        this.addOptions([
                RUMBOOT_COVERAGE: 'Yes'
            ])
    }

    def getCoverageFiles() {
        def fl = []
        builds.each {
            key, value ->
                def tmp = value.getCoverageFile()
                if (steps.fileExists(tmp)) {
                fl += tmp
                }
        }
        return fl
    }

    def tasks(cluster_node, optane_node) {
        def magicks = [:]
        def stages = [:]
        builds.each {
            type,build ->
                def node = cluster_node
                /* Node selection. native doesn't need hardware */

                if (platform != 'native' && type == 'PostProduction') {
                    node = optane_node
                }
                def tg = tag(type)
                stages[tg] = steps.stage(tg) {
                    steps.node(node) {
                        steps.updateGitlabCommitStatus name: tg, state: 'running'
                        try {
                            def tmp = this.options
                            tmp['CMAKE_BUILD_TYPE'] = type
                            tmp['RUMBOOT_PLATFORM'] = platform
                            build.configure(tmp)
                            build.build()

                            if (platform == 'native' || type == 'PostProduction') {
                                build.test(label)
                            }

                            if (this.options['RUMBOOT_COVERAGE'] == 'Yes') {
                                build.coverage()
                            }
                            steps.updateGitlabCommitStatus name: tg, state: 'success'
                            } catch (Exception e) {
                                steps.updateGitlabCommitStatus name: tg, state: 'failed'
                                steps.error('Exception while building project: ' + e.toString())
                        }
                    }
                }
        }
        println("-- stages")
        println(stages)
        println(magicks)
        magicks[platform] = steps.stage(platform) {
                    steps.parallel(stages)
        }

        return magicks
        }

    }

class CheckoutHelper {

    def steps;
    def config = [:]
    def nodes = []

    CheckoutHelper(steps, config) {
        this.steps = steps
        this.config = config
    }

    def checkout() {
        def node = 'master'
        println('Grabbing source @ ' + node)
        if (nodes.contains(node)) {
            println('Already did checkout at @ ' + node)
            return
        }
        nodes += [node]
        println('Checking out')
    }

}
////////////////////////////////////////////////////////////////////
def config = [:]
def cluster_node = 'sim'
def optane_node = 'yacc'
def platforms = ['native', 'basis', 'bbp3', 'oi10', 'mm7705', 'zed', 'nmc', 'scr1']
def coverage = false
def local = false

properties([
    gitLabConnection('git.module.ru'),
    parameters([
        booleanParam(defaultValue: false, name: 'LOCAL_RUN', description: 'Local run via Jenkinsfile-runner')
    ])
])

//TODO: Fill in per-platform options
//
config["basis"] = [
    RUMBOOT_TESTING_PORT: "/dev/serial/by-path/pci-0000:00:1d.2-usb-0:2:1.0-port0",
    RUMBOOT_TESTING_RESETPORT: "2",
    RUMBOOT_DISABLE_TESTING: "No",
    RUMBOOT_COVERAGE: "No"
]

config["mm7705"] = [
    RUMBOOT_TESTING_PORT: "/dev/serial/by-path/pci-0000:00:1d.2-usb-0:1:1.0-port0",
    RUMBOOT_TESTING_RESETPORT: "1",
    RUMBOOT_DISABLE_TESTING: "No",
    RUMBOOT_COVERAGE: "No"
]

config["oi10"] = [
    RUMBOOT_COVERAGE: "No"
]

/////////////////////////////////////////////////////////////////////

builds = [:]
def srcdir = null

stage('Checkout') {
    node(optane_node) {
        ws() {
            checkout scm: [
                        $class: 'GitSCM',
                        branches: scm.branches,
                        doGenerateSubmoduleConfigurations: false,
                        extensions: [[$class: 'SubmoduleOption',
                            disableSubmodules: false,
                            parentCredentials: true,
                            recursiveSubmodules: true,
                            reference: '',
                            trackingSubmodules: false],
                            [$class: 'CleanBeforeCheckout']],
                        submoduleCfg: [],
                        userRemoteConfigs: scm.userRemoteConfigs
                        ]
            srcdir = pwd()
        }
    }
}

platforms.each {
    plat ->
        builds[plat] = new RumBootProject(this, plat, srcdir)
        if (coverage) {
        print('Enabling coverage for ' + plat)
        builds[plat].enableCoverage()
        }
}

config.each {
    key, value ->
        println(key)
        builds[key].addOptions(value)
}

def touchstones = [:]
builds.each {
    plat,build ->
        if (plat == 'native') {
            touchstones += build.tasks(cluster_node, optane_node)
        }
}

println('-------------------')
println(tasks)
println('-------------------')

stage('Touchstone builds (Native)') {
    parallel touchstones
}

def tasks = [:]
builds.each {
    plat,build ->
        if (plat != 'native') {
            tasks += build.tasks(cluster_node, optane_node)
        }
}

println('-------------------')
println(tasks)
println('-------------------')

stage('Build all platforms') {
        parallel tasks
}

stage('Analysis') {
    if (coverage) {
        stage('Merge Coverage Information') {
            node(optane_node) {
                ws() {
                    def fl = []
                    def args = ''
                    builds.each {
                        plat,build ->
                            fl += build.getCoverageFiles()
                    }
                    fl.each {
                        f ->
                            args += ' --add-tracefile ' + f
                    }
                    println(fl)

                    sh('pwd && lcov ' + args + ' -o global.info || true')
                    sh('python ' + srcdir + '/scripts/lcov_cobertura.py' + 'global.info -o coverage.xml')
                }
            }
        }

        stage('Generate Coverage Report') {
            /* TODO */
        }
    }

    stage('Collect test results') {
            /* TODO */
    }
}
