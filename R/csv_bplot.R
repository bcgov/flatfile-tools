args = commandArgs(trailingOnly=TRUE)
if(length(args)==0){
  stop("csv_bplot.R [csv file name]");
}
d = read.csv(file=args[1], header= TRUE)
print(dim(d))

print(length(d[,2]))
png(paste(args[1], "_bplot.png", sep=''))
barplot(d[,2],
	names.arg = d[,1],
	main=paste("Average"),
	xlab=colnames(d)[1],
	ylab=colnames(d)[2])
dev.off()
