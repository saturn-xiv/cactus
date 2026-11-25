from datetime import datetime
from dataclasses import dataclass, asdict

from flask import Flask
from werkzeug.middleware.proxy_fix import ProxyFix

app = Flask(__name__)


@dataclass
class Succeed:
    created_at: datetime

    def __init__(self):
        self.created_at = datetime.now()


@dataclass
class Heartbeat:
    ok: bool
    created_at: datetime

    def __init__(self, ok):
        self.ok = ok
        self.created_at = datetime.now()


@app.get("/api/heartbeat")
def heartbeat():
    return asdict(Heartbeat(True))


def launch(config, port, workers):
    # https://flask.palletsprojects.com/en/stable/deploying/proxy_fix/
    app.wsgi_app = ProxyFix(app.wsgi_app, x_for=1,
                            x_proto=1, x_host=1, x_prefix=1)
