# V1.49 TLS memory recovery test

V1.48 physical evidence: Wi-Fi PASS, DNS PASS, raw TCP 443 PASS, TLS FAIL with `-32512 / SSL - Memory allocation failed`.

V1.49 hypothesis: runtime heap pressure/fragmentation from the animation path may prevent mbedTLS from obtaining a sufficiently large allocation. The build prints free heap and largest block, closes the profile CBP file, frees the 32 KiB heap staging buffer, runs DNS/TCP/TLS and `/v2/account`, then restores the animation resources and revalidates the master CBP handle.

Source SHA-256: 82c37311a669c228792fc4cf7262ff652e2db524c63832c427c262ea37bd40ad
Package SHA-256: 8e71e45224eda537dd515dda2550f837b50de0d73f4fe0a0b4b1affdc3eb43f4

External evidence: Espressif's secure client allocates mbedTLS structures/buffers during `start_ssl_client()`, and Espressif documents dynamic TLS-buffer settings as a memory optimization path. citeturn882650search0turn882650search4
