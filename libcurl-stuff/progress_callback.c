int print_progress(double now, double total)
{
    int perc = 0;
    double i = 0;
    
    fprintf(stderr, "\r");
    
    // calculate percentage only if total is known
    if(total > 0) {
        perc = (int)((now / total) * 100);
        
        // draw progress bar
        fprintf(stderr, "%3d%%",perc);
        fprintf(stderr, "|");
        for (i = 0; i < 100 / 3; i++) {
            if (i == perc / 3)
                fprintf(stderr, "*");
            else
                fprintf(stderr, "-");
        }
        fprintf(stderr, "|");
    }
    
    // now, prepare a nice string
    if(now / 1024 < 1024)
        fprintf(stderr, " %.0f KB", now / 1024);
    else if(now / (1024 * 1024) < 1024)
        fprintf(stderr, " %.2f MB", now / (1024*1024));
    else
        fprintf(stderr, " %.2f GB", now / (1024*1024*1024));
    
    if(total > 0) {
        fprintf(stderr, " /");
        
        if(total / 1024 < 1024)
            fprintf(stderr, " %.0f KB", total / 1024);
        else if(total / (1024 * 1024) < 1024)
            fprintf(stderr, " %.2f MB", total / (1024*1024));
        else
            fprintf(stderr, " %.2f GB", total / (1024*1024*1024));
    }


}

int progress_callback(void *clientp,
                        double dltotal, double dlnow,
                        double ultotal, double ulnow)
{
    
    // determine wether we are uploading or downloading
    if(dltotal > 0 || dlnow > 0)
        print_progress(dltotal, dlnow);
    else if (ultotal > 0 || ulnow > 0)
        print_progress(ultotal, ulnow);
    else
        fprintf(stderr, "\rUnknown Progress...");
    
    return 0;
}
