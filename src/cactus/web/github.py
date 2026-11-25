from dataclasses import asdict

from . import app, Succeed


@app.post("/api/github/webhooks/<id>")
def webhook(id):
    app.logger.info("receive github webhook")
    return asdict(Succeed())
