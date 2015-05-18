# convert the Marxan matrix into it's sparse matrix representations


sWorkingDir <- "/Users/matt/Documents/Bugs/convert_mtx_15May2015/"


sInMatrix <- paste0(sWorkingDir,"puvspr2.csv")
sOutPuorder <- paste0(sWorkingDir,"puorder.csv")
sOutSporder <- paste0(sWorkingDir,"sporder.csv")

matrix <- read.csv(sInMatrix,stringsAsFactors = FALSE)

# generate sporder.dat
write('species,pu,amount',file=sOutSporder)
for (i in 2:ncol(matrix))
{
  for (j in 1:nrow(matrix))
  {
    rAmount <- matrix[j,i]
    
    if (rAmount > 0)
    {
      iPUID <- matrix[j,1]
      iSPID <- substring(colnames(matrix)[i],2)
    
      write(paste(iSPID,iPUID,rAmount,sep=","),
            file=sOutSporder,append=TRUE)
    }
  }
}

# generate puorder.dat
write('species,pu,amount',file=sOutPuorder)
for (j in 1:nrow(matrix))
{
  for (i in 2:ncol(matrix))
  {
    rAmount <- matrix[j,i]
    
    if (rAmount > 0)
    {
      iPUID <- matrix[j,1]
      iSPID <- substring(colnames(matrix)[i],2)
      
      write(paste(iSPID,iPUID,rAmount,sep=","),
            file=sOutPuorder,append=TRUE)
    }
  }
}


