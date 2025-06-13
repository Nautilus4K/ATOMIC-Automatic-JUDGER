/* timezone_stub.c */
/* 
 * This has been created to fix linking errors between Qt and OpenSSL
 * It will create dummy exports for the old CRT 'timezone' variable
 */

#ifdef __cplusplus
extern "C" {
#endif

/* the “real” old‐CRT globals */
long timezone  = 0;
long _timezone = 0;

/* the import‐pointer that the code is referencing */
long * __imp___timezone = &timezone;

#ifdef __cplusplus
}
#endif
