typedef enum LogErrors{
   LOG_MALLOC,
   LOG_READ,
   LOG_WRITE
} LogErrors;

/// @brief Logs the error that may occur in the programm
/// @param error the error type
/// @param log_location 1 to stdout, 2 to stderr, 3 to file
void log_error(LogErrors error, int log_location);