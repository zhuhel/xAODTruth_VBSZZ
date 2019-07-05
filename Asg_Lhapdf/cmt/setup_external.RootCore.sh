if test -z "${LHAPDF_DATA_PATH:+x}"
then
    export LHAPDF_DATA_PATH
fi

export LHAPDF_DATA_PATH=$ROOTCOREBIN/data/Asg_Lhapdf_LHAPDF:/cvmfs/sft.cern.ch/lcg/external/lhapdfsets/current/:$LHAPDF_DATA_PATH
