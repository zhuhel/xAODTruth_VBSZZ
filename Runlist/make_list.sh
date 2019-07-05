#sdir="/gpfs/mnt/atlasgpfs01/usatlas/bnl_local/hzhu/MCgenerator/MG5/DoShowering/"
sdir="/pnfs/usatlas.bnl.gov/users/hzhu/rucio/"

out=list_MG

if [ ! -d List ]; then mkdir List; fi

for ds in `ls -d ${sdir}/*364250*`; do

  if [ -d ${ds} ]; then
    nf=0
    ds0=`basename ${ds}`
    list=List/${ds0}.list
    for file in `ls ${ds}/*root*`; do
      nf=$((nf+1))
      readlink -f ${file} >> ${list}
    done 
    if [ ${nf} -eq 0 ]; then
      echo "Warning=> no files found at ${ds}"
      continue
    fi
    
    less ${list} | sort | uniq > ${list}.tmp
    mv ${list}.tmp ${list}
  
    readlink -f ${list} >> ${out}
  fi
done
