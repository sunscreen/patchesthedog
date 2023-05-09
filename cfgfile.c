/* reread config nothing much changed just additional debug statements used to trace how far it was gettings in the code execution */
 
void config_reread_config()
{
    int           ret;
    ice_config_t *config;
    ice_config_t  new_config;
    /* reread config file */
    config = config_grab_config(); /* Both to get the lock, and to be able */
    xmlSetGenericErrorFunc("config", log_parse_failure);
    ret = config_parse_file(config->config_filename, &new_config);
    if(ret < 0) {
        ICECAST_LOG_ERROR("Error parsing config, not replacing existing config");
        switch (ret) {
            case CONFIG_EINSANE:
                ICECAST_LOG_ERROR("Config filename null or blank");
            break;
            case CONFIG_ENOROOT:
                ICECAST_LOG_ERROR("Root element not found in %s", config->config_filename);
            break;
            case CONFIG_EBADROOT:
                ICECAST_LOG_ERROR("Not an icecast2 config file: %s",
                        config->config_filename);
            break;
            default:
                ICECAST_LOG_ERROR("Parse error in reading %s", config->config_filename);
            break;
        }
        config_release_config();
    } else {
        config_clear(config);
        config_set_config(&new_config);
 
        config = config_get_config_unlocked();
 
        restart_logging(config);
        printf("config_reload: restarting logging...\n");
 
        prng_configure(config);
        printf("config_reload: prng\n");
 
        main_config_reload(config);
        printf("config_reload: main\n");
 
        connection_reread_config(config);
        printf("config_reload: connection config\n");
 
        yp_recheck_config(config);
        printf("config_reload: yp config\n");
 
        fserve_recheck_mime_types(config);
        printf("config_reload: fserve mime types\n");
 
        stats_global(config);
        printf("config_reload: stats_global\n");
 
        config_release_config();
        slave_update_all_mounts();
        printf("config_reload: all mounts updated...\n");
 
        xslt_clear_cache();
    }
}
