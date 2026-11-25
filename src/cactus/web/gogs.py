from dataclasses import asdict

from . import app, Succeed


@app.post("/api/gogs/webhooks/<id>")
def webhook(id):
    app.logger.info("receive gogs webhook")
    return asdict(Succeed())
