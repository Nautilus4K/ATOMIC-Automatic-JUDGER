# This script is built to run the WSGI Interface. This does not have any
# affliation with the Apache HTTP Server Project or anthing related but
# a way to run WSGI Interface using waitress.

# Nickname? Hell yea
# Valkyrie.

from waitress import serve
from wsgi_interface import application

serve(application, host='0.0.0.0', port=80, ident="Valkyrie", url_scheme="https")
