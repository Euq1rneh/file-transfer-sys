#include "logger.h"

void write_log(const char* error_msg, int log_location){
    switch (log_location)
    {
    case 1:
        
        break;
    case 2:
        
        break;
    case 3:
        
        break;
    default:
        break;
    }
}

void log_error(LogErrors error, int log_location)
{
    switch (error)
    {
    case LOG_MALLOC:
        write_log("Error allocating memory\n", log_location);
        break;
    case LOG_READ:
        write_log("Error reading data\n", log_location);
        break;
    case LOG_WRITE:
        write_log("Error writting data\n", log_location);
        break;
    default:
        break;
    }
}