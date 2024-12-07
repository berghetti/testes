#!/bin/bash

install_nodejs()
{
  curl -sL install-node.vercel.app/lts | sudo bash -s -- --yes
}

install_requeriments()
{
  sudo apt update
  sudo apt install -y git xz-utils ripgrep clang-format
  install_nodejs
}

install_neovim()
{
  if [ -x /opt/nvim-linux64/bin/nvim ]; then
    return
  fi

  #curl -LOs https://github.com/neovim/neovim/releases/latest/download/nvim-linux64.tar.gz
  curl -LOs https://github.com/neovim/neovim-releases/releases/download/v0.10.2/nvim-linux64.tar.gz
  sudo rm -rf /opt/nvim
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

install_requeriments
install_neovim
install_conf
install_plug
