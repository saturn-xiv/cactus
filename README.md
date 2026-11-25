# CACTUS - Webhooks server for Gogs & Github

## Usage

```bash
# Initial python3 virtual env
$ sudo apt install python3-full python3-dev build-essential
$ python3 -m venv $PWD/tmp/python

# Load virtual env vars
$ source $PWD/tmp/python/bin/activate

# Install dependencies for development
> python3 -m pip install -e .

# show help
> python3 -m cactus -h

# start a http server
> python3 -m cactus -d web -p 8080


# building for production
> pip install build
> python3 -m build

> pip install gunicorn

# Install for production
> pip install dist/cactus-0.0.post1.dev1+g09d259a81.d20261125-py3-none-any.whl
> cactus -h
```

## Documents

- [Github](https://docs.github.com/en/webhooks/about-webhooks)
- [Gogs](https://gogs.io/advancing/webhooks)
- [Nginx](https://gunicorn.org/deploy/#nginx-configuration)
- [Gunicorn](https://gunicorn.org/quickstart/)
