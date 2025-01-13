set (
    Header
    "WIFI"
)
set (
    Source
    "WIFI/WIFI.c"
)
set (
    Cert
    "CERT/github_cert.pem"
)

idf_component_register(SRCS "main.c" ${Source}
                    INCLUDE_DIRS "." ${Header}
                    EMBED_TXTFILES ${Cert})