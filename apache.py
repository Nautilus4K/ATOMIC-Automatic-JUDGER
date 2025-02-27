from waitress import serve
from wsgi_interface import application

serve(application, host='0.0.0.0', port=80, ident="ATOMICAutomation", url_scheme="https")
