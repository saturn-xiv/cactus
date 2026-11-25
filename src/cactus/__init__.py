import os
import logging
import argparse
import tomllib
from importlib.metadata import version, metadata
from concurrent.futures import ThreadPoolExecutor

from .web import launch as launch_web_server


logger = logging.getLogger(__name__)


def main():
    meta = metadata(__package__)
    parser = argparse.ArgumentParser(
        description=meta["Summary"], formatter_class=argparse.ArgumentDefaultsHelpFormatter)

    parser.add_argument(
        '-c', '--config', default='config.toml', help="config file path")
    parser.add_argument('-d', '--debug',
                        action='store_true', help='run on debug mode')
    parser.add_argument('-v', '--verbose',
                        action='version', version=version(__package__))

    sub_parsers = parser.add_subparsers(
        dest="command", required=True, help="Available sub-commands")

    parser_web = sub_parsers.add_parser("web", help="Launch a HTTP server")
    parser_web.add_argument('-p', '--port', type=int, default=8080)
    parser_web.add_argument('-w', '--workers', type=int, default=4)

    args = parser.parse_args()
    logging.basicConfig(
        format='%(asctime)s %(levelname).1s %(message)s', level=logging.DEBUG if args.debug else logging.INFO)
    logger.debug("running on debug mode")
    if args.debug:
        logging.getLogger("pika").setLevel(logging.INFO)

    logger.debug("load configuration from %s", args.config)
    with open(args.config, "rb") as file:
        config = tomllib.load(file)
    if args.command == "http":
        launch_web_server(config, args.port, args.workers)
