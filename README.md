# CACTUS - Webhooks server for Gogs & Github

## Development

    ```bash
    git clone https://github.com/saturn-xiv/cactus.git $HOME/workspace/cactus
    cd $HOME/workspace/cactus/

    xmake update
    xmake repo -u

    ./build.sh
    ```

## Deployment

    ```bash
    cactus -d http -p 10002 -c config.toml
    ```

## Documents

- [Github webhooks](https://docs.github.com/en/webhooks/about-webhooks)
- [Gogs webhooks](https://gogs.io/advancing/webhooks)
