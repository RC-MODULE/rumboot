#!/bin/bash -x

while [[ $# -gt 0 ]]; do
  case $1 in
    --num_iterations_file)
      NA_TEST_num_iterations_file=$2
          shift
         shift
            ;;
    --in_file)
                  NA_TEST_in_file=$2
          shift
         shift
            ;;
    --in_ameba_file)
            NA_TEST_in_ameba_file=$2
          shift
         shift
            ;;
    --in_with_unused_file)
      NA_TEST_in_with_unused_file=$2
          shift
         shift
            ;;
    --warr_file)
                NA_TEST_warr_file=$2
          shift
         shift
            ;;
    --warr_with_unused_file)
    NA_TEST_warr_with_unused_file=$2
          shift
         shift
            ;;
    --etalon_file)
              NA_TEST_etalon_file=$2
          shift
         shift
            ;;
    --etalon_ameba_file)
        NA_TEST_etalon_ameba_file=$2
          shift
         shift
            ;;
    --etalon_with_unused_file)
  NA_TEST_etalon_with_unused_file=$2
          shift
         shift
            ;;
    --op0_vec_file)
             NA_TEST_op0_vec_file=$2
          shift
         shift
            ;;
    --op0_cube_file)
            NA_TEST_op0_cube_file=$2
          shift
         shift
            ;;
    --op0_ameba_file)
           NA_TEST_op0_ameba_file=$2
          shift
         shift
            ;;
    --op0_with_unused_file)
     NA_TEST_op0_with_unused_file=$2
          shift
         shift
            ;;
    --op1_vec_file)
             NA_TEST_op1_vec_file=$2
          shift
         shift
            ;;
    --op1_cube_file)
            NA_TEST_op1_cube_file=$2
          shift
         shift
            ;;
    --op1_ameba_file)
           NA_TEST_op1_ameba_file=$2
          shift
         shift
            ;;
    --op1_with_unused_file)
     NA_TEST_op1_with_unused_file=$2
          shift
         shift
            ;;
    --op2_vec_file)
             NA_TEST_op2_vec_file=$2
          shift
         shift
            ;;
    --op2_cube_file)
            NA_TEST_op2_cube_file=$2
          shift
         shift
            ;;
    --op2_ameba_file)
           NA_TEST_op2_ameba_file=$2
          shift
         shift
            ;;
    --op2_with_unused_file)
     NA_TEST_op2_with_unused_file=$2
          shift
         shift
            ;;
    --lut1_file)
                NA_TEST_lut1_file=$2
          shift
         shift
            ;;
    --lut2_file)
                NA_TEST_lut2_file=$2
          shift
         shift
            ;;
    --cfg_file)
                 NA_TEST_cfg_file=$2
          shift
         shift
            ;;
    --cfg_mpe_file)
             NA_TEST_cfg_mpe_file=$2
          shift
         shift
            ;;
    --cfg_ppe_file)
             NA_TEST_cfg_ppe_file=$2
          shift
         shift
            ;;
    --status_regs_file)
         NA_TEST_status_regs_file=$2
          shift
         shift
            ;;
    *)
       shift
       ;;
  esac
done


# Metrics

find . -maxdepth 1 -type f -name ${NA_TEST_in_file}.\*.metrics | sort -V | xargs cat > merged.${NA_TEST_in_file}.metrics && rm ${NA_TEST_in_file}.*.metrics

if compgen -G "${NA_TEST_warr_file}.*.metrics" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_warr_file}.\*.metrics | sort -V | xargs cat > merged.${NA_TEST_warr_file}.metrics && rm ${NA_TEST_warr_file}.*.metrics
fi

find . -maxdepth 1 -type f -name ${NA_TEST_etalon_file}.\*.metrics | sort -V | xargs cat > merged.${NA_TEST_etalon_file}.metrics && rm ${NA_TEST_etalon_file}.*.metrics

if compgen -G "${NA_TEST_op0_vec_file}.*.metrics" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op0_vec_file}.\*.metrics | sort -V | xargs cat > merged.${NA_TEST_op0_vec_file}.metrics && rm ${NA_TEST_op0_vec_file}.*.metrics
fi
if compgen -G "${NA_TEST_op0_cube_file}.*.metrics" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op0_cube_file}.\*.metrics | sort -V | xargs cat > merged.${NA_TEST_op0_cube_file}.metrics && rm ${NA_TEST_op0_cube_file}.*.metrics
fi
if compgen -G "${NA_TEST_op1_vec_file}.*.metrics" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op1_vec_file}.\*.metrics | sort -V | xargs cat > merged.${NA_TEST_op1_vec_file}.metrics && rm ${NA_TEST_op1_vec_file}.*.metrics
fi
if compgen -G "${NA_TEST_op1_cube_file}.*.metrics" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op1_cube_file}.\*.metrics | sort -V | xargs cat > merged.${NA_TEST_op1_cube_file}.metrics && rm ${NA_TEST_op1_cube_file}.*.metrics
fi
if compgen -G "${NA_TEST_op2_vec_file}.*.metrics" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op2_vec_file}.\*.metrics | sort -V | xargs cat > merged.${NA_TEST_op2_vec_file}.metrics && rm ${NA_TEST_op2_vec_file}.*.metrics
fi
if compgen -G "${NA_TEST_op2_cube_file}.*.metrics" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op2_cube_file}.\*.metrics | sort -V | xargs cat > merged.${NA_TEST_op2_cube_file}.metrics && rm ${NA_TEST_op2_cube_file}.*.metrics
fi
if compgen -G "${NA_TEST_lut1_file}.*.metrics" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_lut1_file}.\*.metrics | sort -V | xargs cat > merged.${NA_TEST_lut1_file}.metrics && rm ${NA_TEST_lut1_file}.*.metrics
fi
if compgen -G "${NA_TEST_lut2_file}.*.metrics" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_lut2_file}.\*.metrics | sort -V | xargs cat > merged.${NA_TEST_lut2_file}.metrics && rm ${NA_TEST_lut2_file}.*.metrics
fi

# Data

find . -maxdepth 1 -type f -name ${NA_TEST_in_file}.\* | sort -V | xargs cat > merged.${NA_TEST_in_file} && rm ${NA_TEST_in_file}.*

if compgen -G "${NA_TEST_in_ameba_file}.*" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_in_ameba_file}.\* | sort -V | xargs cat > merged.${NA_TEST_in_ameba_file} && rm ${NA_TEST_in_ameba_file}.*
fi

if compgen -G "${NA_TEST_warr_file}.*" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_warr_file}.\* | sort -V | xargs cat > merged.${NA_TEST_warr_file} && rm ${NA_TEST_warr_file}.*
fi

find . -maxdepth 1 -type f -name ${NA_TEST_etalon_file}.\* | sort -V | xargs cat > merged.${NA_TEST_etalon_file} && rm ${NA_TEST_etalon_file}.*

if compgen -G "${NA_TEST_op0_vec_file}.*" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op0_vec_file}.\* | sort -V | xargs cat > merged.${NA_TEST_op0_vec_file} && rm ${NA_TEST_op0_vec_file}.*
fi
if compgen -G "${NA_TEST_op0_cube_file}.*" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op0_cube_file}.\* | sort -V | xargs cat > merged.${NA_TEST_op0_cube_file} && rm ${NA_TEST_op0_cube_file}.*
fi
if compgen -G "${NA_TEST_op1_vec_file}.*" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op1_vec_file}.\* | sort -V | xargs cat > merged.${NA_TEST_op1_vec_file} && rm ${NA_TEST_op1_vec_file}.*
fi
if compgen -G "${NA_TEST_op1_cube_file}.*" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op1_cube_file}.\* | sort -V | xargs cat > merged.${NA_TEST_op1_cube_file} && rm ${NA_TEST_op1_cube_file}.*
fi
if compgen -G "${NA_TEST_op2_vec_file}.*" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op2_vec_file}.\* | sort -V | xargs cat > merged.${NA_TEST_op2_vec_file} && rm ${NA_TEST_op2_vec_file}.*
fi
if compgen -G "${NA_TEST_op2_cube_file}*" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_op2_cube_file}.\* | sort -V | xargs cat > merged.${NA_TEST_op2_cube_file} && rm ${NA_TEST_op2_cube_file}.*
fi
if compgen -G "${NA_TEST_lut1_file}.*" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_lut1_file}.\* | sort -V | xargs cat > merged.${NA_TEST_lut1_file} && rm ${NA_TEST_lut1_file}.*
fi
if compgen -G "${NA_TEST_lut2_file}.*" > /dev/null ; then
find . -maxdepth 1 -type f -name ${NA_TEST_lut2_file}.\* | sort -V | xargs cat > merged.${NA_TEST_lut2_file} && rm ${NA_TEST_lut2_file}.*
fi


# Config

if compgen -G "${NA_TEST_cfg_file}.*" > /dev/null ; then
  find . -maxdepth 1 -type f -name ${NA_TEST_cfg_file}.\* | sort -V | xargs cat > merged.${NA_TEST_cfg_file} && rm ${NA_TEST_cfg_file}.*
fi
if compgen -G "${NA_TEST_cfg_mpe_file}.*" > /dev/null ; then
  find . -maxdepth 1 -type f -name ${NA_TEST_cfg_mpe_file}.\* | sort -V | xargs cat > merged.${NA_TEST_cfg_mpe_file} && rm ${NA_TEST_cfg_mpe_file}.*
fi
if compgen -G "${NA_TEST_cfg_ppe_file}.*" > /dev/null ; then
  find . -maxdepth 1 -type f -name ${NA_TEST_cfg_ppe_file}.\* | sort -V | xargs cat > merged.${NA_TEST_cfg_ppe_file} && rm ${NA_TEST_cfg_ppe_file}.*
fi
  
if compgen -G "${NA_TEST_status_regs_file}.*" > /dev/null ; then
  find . -maxdepth 1 -type f -name ${NA_TEST_status_regs_file}.\* | sort -V | xargs cat > merged.${NA_TEST_status_regs_file} && rm ${NA_TEST_status_regs_file}.*
fi

# Turn them back
for file in `find . -maxdepth 1 -name "merged.*"`; do
        mv "$file" "${file/merged./}"
done