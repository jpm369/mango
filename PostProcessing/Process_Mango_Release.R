library(tcltk)



varEntryDialog <- function(vars, 
                           labels = vars,
                           fun = rep(list(as.character), length(vars)),
                           title = 'Variable Entry',
                           prompt = NULL) {
  require(tcltk)
  
  stopifnot(length(vars) == length(labels), length(labels) == length(fun))
  
  # Create a variable to keep track of the state of the dialog window:
  # done = 0; If the window is active
  # done = 1; If the window has been closed using the OK button
  # done = 2; If the window has been closed using the Cancel button or destroyed
  done <- tclVar(0)
  
  tt <- tktoplevel()
  tkwm.title(tt, title)	
  entries <- list()
  tclvars <- list()
  
  # Capture the event "Destroy" (e.g. Alt-F4 in Windows) and when this happens, 
  # assign 2 to done.
  tkbind(tt,"<Destroy>",function() tclvalue(done)<-2)
  
  for(i in seq_along(vars)) {
    tclvars[[i]] <- tclVar("")
    entries[[i]] <- tkentry(tt, textvariable=tclvars[[i]])
  }
  
  doneVal <- as.integer(tclvalue(done))
  results <- list()
  
  reset <- function() {
    for(i in seq_along(entries)) {
      tclvalue(tclvars[[i]]) <<- ""
    }
  }
  reset.but <- tkbutton(tt, text="Reset", command=reset)
  
  cancel <- function() {
    
    tclvalue(done) <- 2
  }
  cancel.but <- tkbutton(tt, text='Cancel', command=cancel)
  
  submit <- function() {
    for(i in seq_along(vars)) {
      tryCatch( {
        results[[vars[[i]]]] <<- fun[[i]](tclvalue(tclvars[[i]]))
        tclvalue(done) <- 1
      },
      error = function(e) { tkmessageBox(message=geterrmessage()) },
      finally = { }
      )
    }
  }
  submit.but <- tkbutton(tt, text="Submit", command=submit)
  
  if(!is.null(prompt)) {
    tkgrid(tklabel(tt,text=prompt), columnspan=3, pady=10)
  }
  
  for(i in seq_along(vars)) {
    tkgrid(tklabel(tt, text=labels[i]), entries[[i]], pady=10, padx=10, columnspan=4)
  }
  
  tkgrid(submit.but, cancel.but, reset.but, pady=10, padx=10, columnspan=3)
  tkfocus(tt)
  
  # Do not proceed with the following code until the variable done is non-zero.
  #   (But other processes can still run, i.e. the system is not frozen.)
  tkwait.variable(done)
  
  if(tclvalue(done) != 1) {
    results <- NULL
  }
  
  tkdestroy(tt)
  return(results)
}
workingdir <- tk_choose.dir(getwd(), "Choose a directory containing at least 1 pep.xml.txt file from mango")

vals <- varEntryDialog(vars=c('Var1'), labels=c('Pick an FDR cutoff between 0 and 1.'), fun=c(
  function(x) {
    x <- as.numeric(x)
    if(x >= 0 & x <= 1) {
      return(x)
    } else {
      stop("Seriously it's not that hard.\nPlease enter a number between 0 and 1.")
    }
  } ))

FDRcutoff <- vals$Var1

setwd(workingdir)
mango_files <- list.files(path=workingdir, pattern = ".\\.txt$")

filex <- length(mango_files)
mango_summary <- NULL

getxlinktype <- function(x,y,u,v){
  if(x==y){
    return("homodimer")
  } else if(u==v){
    return("intralink")
  } else {
    return("interlink")
  }
}

pepsort <- function(pep1,pep2){
  if(pep1<pep2){
    return(paste(pep1,pep2,sep="_"))
  } else {
    return(paste(pep2,pep1,sep="_"))
  }
}

eval2number <- function(x){
  as.numeric(strsplit(x,split="E")[[1]][1])*10^(as.numeric(strsplit(x,split="E")[[1]][2]))
}



checkpairs <- function(a, b) {
  s.a <- strsplit(a, "")[[1]]
  s.b <- strsplit(b, "")[[1]]
  paste(s.a[s.a != s.b], collapse = "")
}


for(currentfile in 1:filex){
  print(mango_files[currentfile])
  mydata <- read.table(mango_files[currentfile],stringsAsFactors = F,header=T,skip=1)
  #sort by scan then spec.name to pair adjacent columns
  sorted <- mydata[order(mydata$scan,mydata$spectrum),]
  
  paircheck <- regmatches(sorted$spectrum, regexpr(".{19}$", sorted$spectrum))
  paircheck <- substr(paircheck,0,nchar(paircheck)-2)
  valuestoremove <- NULL
  
  for(i in 1:(length(sorted$scan)-1))
  {
    if(i==1){
      j=0
    }
    if(j==1){
      j=0
      next
    }
    if(checkpairs(paircheck[i],paircheck[i+1])[1]=="A"){
      j=1
    }else{
      valuestoremove <- c(valuestoremove,i)
    }
  }
  
  if(length(valuestoremove)>0){
    sorted <- sorted[-valuestoremove,]
  }
  
  
  
  
  paired <- as.data.frame(matrix(0,ncol=dim(sorted)[2]*2,nrow=dim(sorted)[1]/2))
  pep1 <- sorted[seq(1,dim(sorted)[1],2),]
  pep2 <- sorted[seq(2,dim(sorted)[1],2),]
  colnames(pep1) <- paste("pep1",colnames(sorted),sep=".")
  colnames(pep2) <- paste("pep2",colnames(sorted),sep=".")
  
  
  
  paired <- cbind(pep1,pep2)
  
  paired <- cbind(paired,maxeval=0)
  
  #strike out things without an evalue
  paired <- paired[!(paired$pep1.e.value=="-"|paired$pep2.e.value=="-"),]
  
  #calculate the max for the pair
  paired$maxeval <- pmax(paired$pep1.e.value,paired$pep2.e.value)
  
  #sort by evalue, good at the top, bad at bottom
  paired <- paired[order(paired$maxeval),]
  
  #make a unique label for later
  paired$pepstring <- 0
  paired$protpair <- 0
  paired$xlinktype <- 0
  paired <- cbind(paired,decoy=0,sumdecoys=0,fdr=0)
  
  #check for scan collisions
  #sort by scan then eval
  paired <- paired[order(paired$pep1.scan, paired$maxeval),]
  
  #remove all but first instance of scan in this sort (has best eval)
  paired <- paired[!duplicated(paired$pep1.scan),]
  paired <- cbind(src=mango_files[currentfile],paired)
  mango_summary <- rbind(mango_summary,paired)
}

paired <- mango_summary
paired <- paired[order(paired$maxeval),]
paired$decoy <- as.numeric(substr(paired$pep1.protein,1,3)=="rev"|substr(paired$pep2.protein,1,3)=="rev")
paired$sumdecoys <- cumsum(paired$decoy)
paired$fdr <- paired$sumdecoys/seq(1,length(paired$sumdecoys),1)

passingresults <- paired[paired$fdr<FDRcutoff,]
if(length(passingresults$fdr>0)){
  
  modpep1 <- substr(passingresults$pep1.modified_peptide,3,nchar(passingresults$pep1.modified_peptide)-2)
  modpep2 <- substr(passingresults$pep2.modified_peptide,3,nchar(passingresults$pep2.modified_peptide)-2)
  passingresults$pepstring <- mapply(pepsort,modpep1,modpep2)
  
  
  for(i in 1:length(passingresults$protpair)){
    if(passingresults$pep1.plain_peptide[i]<passingresults$pep2.plain_peptide[i]){
      passingresults$protpair[i] <- paste(strsplit(passingresults$pep1.protein[i],split="\\s+")[[1]][1],strsplit(passingresults$pep2.protein[i],split="\\s+")[[1]][1],sep="_")
    }else{
      passingresults$protpair[i] <- paste(strsplit(passingresults$pep2.protein[i],split="\\s+")[[1]][1],strsplit(passingresults$pep1.protein[i],split="\\s+")[[1]][1],sep="_")
    }
  }
  
  passingresults$xlinktype <- mapply(getxlinktype,passingresults$pep1.modified_peptide,passingresults$pep2.modified_peptide,passingresults$pep1.protein,passingresults$pep2.protein)
  
  #clip out decoy hits
  passingresults <- passingresults[!(substr(passingresults$pep1.protein,1,3)=="rev"|substr(passingresults$pep2.protein,1,3)=="rev"),]
}
passingresults$pep1.ions <- paste("'",passingresults$pep1.ions,sep="")
passingresults$pep2.ions <- paste("'",passingresults$pep2.ions,sep="")

write.table(passingresults,file = "mangoSummary_allfiles.xls",sep="\t",row.names = FALSE,quote = FALSE)

