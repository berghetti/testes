#!/bin/bash
set -x

# Ubuntu
VERSION=$(grep -oP '(?<=VERSION_ID=")\d+' /etc/os-release)

install_nodejs()
{
  if [[ $VERSION -lt 20 ]]; then
    curl -sL install-node.vercel.app/16 | sudo bash -s -- --yes
  else
    curl -sL install-node.vercel.app/22 | sudo bash -s -- --yes
  fi
}

install_requeriments()
{
  sudo apt update
  
  if [[ $VERSION -lt 20 ]]; then
    sudo apt install -y git xz-utils clang-format
    curl -LOs https://github.com/BurntSushi/ripgrep/releases/download/14.0.3/ripgrep_14.0.3-1_amd64.deb
    sudo apt install ./ripgrep_14.0.3-1_amd64.deb
    rm ripgrep_14.0.3-1_amd64.deb
  else
    sudo apt install -y git xz-utils ripgrep clang-format
  fi
  install_nodejs
}

install_neovim()
{
  if [[ $VERSION -lt 20 ]]; then
    NVIM="https://github.com/neovim/neovim/releases/download/v0.7.2/nvim-linux64.tar.gz"
  else
    NVIM="https://github.com/neovim/neovim/releases/download/v0.10.2/nvim-linux64.tar.gz"
  fi
  curl -LOs $NVIM
  sudo rm -rf /opt/nvim/
  sudo tar -C /opt -xzf nvim-linux64.tar.gz
  sudo rm nvim-linux64.tar.gz

  echo 'export PATH="$PATH:/opt/nvim-linux64/bin"' >> ~/.bashrc
  echo "alias v='nvim'" >> ~/.bashrc
  . ~/.bashrc
}

install_plug()
{
  sh -c 'curl -fLo "${XDG_DATA_HOME:-$HOME/.local/share}"/nvim/site/autoload/plug.vim --create-dirs \
       https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim' \
  && \
  /opt/nvim-linux64/bin/nvim -c "PlugInstall" -c "qa" \
  && \
  /opt/nvim-linux64/bin/nvim -c "CocInstall" -c "qa"
}

install_conf()
{
  curl -LOs https://github.com/berghetti/testes/raw/master/nvim/.vimrc
  mv .vimrc ~/.vimrc
  mkdir -p ~/.config/nvim/
  echo -e 'set runtimepath^=~/.vim runtimepath+=~/.vim/after\n'\
	  'let &packpath = &runtimepath\n'\
	  'source ~/.vimrc' > ~/.config/nvim/init.vim

}

pushd /tmp

install_requeriments
install_neovim
install_conf
install_plug

popd
