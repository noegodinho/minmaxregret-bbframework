library(gtools)
library(MASS)

plotgraphics = function(firstfolder, sequence, name, secondfolder = NULL, 
                        thirdfolder = NULL, legendX = NULL, 
                        legendY = NULL, legendZ = NULL, ylim = NULL, 
                        len = NULL, titleX = 'Number of vertices'){

   
    files = list.files(path = firstfolder, full.names = TRUE)
    f_v = c()
    files = mixedsort(files)

    if(is.null(len)){
        len = sequence - 3
    }

    files_vector = len

    for(i in len){
        v = read.table(files[i])$V1
        files_vector[i] = mean(v)
        f_v = c(f_v, v)
    }

    pdf(paste0('images/', name, '.pdf'))
    plot(sequence, files_vector, type="l", ylim = ylim, log="y",
         col = 'blue', ann = FALSE)
    
    files2 = list.files(path = secondfolder, full.names = TRUE)
    files_vector2 = len
    f_v2 = c()
    files2 = mixedsort(files2)

    for(i in len){
        v = read.table(files2[i])$V1
        files_vector2[i] = mean(v)
        f_v2 = c(f_v2, v)
    }

    lines(sequence, files_vector2, type = 'l', col = 'red')
    
    if(is.null(thirdfolder)){
        legend('topleft', legend = c(legendX, legendY),
                col = c('blue','red'), pch = c(15,20))
    }
    
    title(xlab = titleX)
    title(ylab = 'Time (in seconds)')    

    if(!is.null(thirdfolder)){
        files3 = list.files(path = thirdfolder, full.names = TRUE)
        files_vector3 = len
        f_v3 = c()
        files3 = mixedsort(files3)

        for(i in len){
            v = read.table(files3[i])$V1
            files_vector3[i] = mean(v)
            f_v3 = c(f_v3, v)
        }

        lines(sequence, files_vector3, type = 'l', col = 'green')

        legend('topleft', legend = c(legendX, legendY, legendZ),
                col = c('blue','red', 'green'), pch = c(15,20))
    }

    dev.off()

    s = sort(rep(sequence, 100))
    #l = lm(f_v~s)
    # check the best transformation
    # see where the pick is located: 
    #	1 -> linear
    #   0.5 -> sqrt(f_v)
    #   0 -> log(f_v)
    #boxcox(l)

    # in this example, sqrt is the best -> do summary(l) and 
    # check R-squared - it should be larger than 0.9.   
    # it suggest that the time is quadratic with respect the number of arcs.
    l = lm(log(f_v)~s)
    print(firstfolder)
    print(summary(l))

    pdf(paste0('images/', name, firstfolder, '.pdf'))
    plot(s, log(f_v),cex =.1,
         col = 'blue', ann = FALSE)

    abline(l, col = 'orange')
    grid()
    title(xlab = titleX)
    title(ylab = 'log(Time)')
    dev.off()

    l = lm(log(f_v2)~s)
    print(secondfolder)
    print(summary(l))

    pdf(paste0('images/', name, secondfolder, '.pdf'))
    plot(s, log(f_v2),cex =.1,
         col = 'red', ann = FALSE)

    abline(l, col = 'orange')
    grid()
    title(xlab = titleX)
    title(ylab = 'log(Time)')
    dev.off()

    if(!is.null(thirdfolder)){
        l = lm(log(f_v3)~s)
        print(thirdfolder)
        print(summary(l))

        pdf(paste0('images/', name, thirdfolder, '.pdf'))
        plot(s, log(f_v3),cex =.1,
             col = 'green', ann = FALSE)

        abline(l, col = 'orange')
        grid()
        title(xlab = titleX)
        title(ylab = 'log(Time)')
        dev.off()
    }
}

plotgraphics(firstfolder = 'new_b_b_bound12initialsort', secondfolder = 'new_b_b_bound12initialthread',
             sequence = 4:15, name = 'initial&sort&thread',
             legendY = 'Bound 1&2 with Initial Solution and Thread', legendX = 'Bound 1&2 with Initial Solution and Sort', 
             legendZ = 'Bound 1&2 with Initial Solution, Sort and Thread', thirdfolder = 'new_b_b_bound12initialsortthread', 
             ylim = c(0.00005, 1200))

plotgraphics(firstfolder = 'new_b_b_bound1', secondfolder = 'new_b_b_bound2',
             sequence = 4:13, name = 'bound1_2_12',
             legendX = 'Bound 1', legendY = 'Bound 2', legendZ = 'Bound 1&2',
             thirdfolder = 'new_b_b_bound12', ylim = c(0.00005, 1e+03))
