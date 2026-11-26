# CACTUS - Webhooks server for Gogs & Github

## Development

    ```bash
    git clone https://github.com/saturn-xiv/cactus.git $HOME/workspace/cactus
    cd $HOME/workspace/cactus/
    
    # https://xmake.io/guide/quick-start.html#installation
    sudo apt install build-essential crossbuild-essential-amd64 crossbuild-essential-arm64 crossbuild-essential-riscv64
    curl -fsSL https://xmake.io/shget.text | bash
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
