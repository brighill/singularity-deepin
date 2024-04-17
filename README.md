# Singularity-wechat

## 快速开始

1. 下载微信镜像

   ```bash
   sudo wget https://github.com/brighill/singularity-deepin/releases/download/stable/wechat.sif -O /opt/wechat.sif
   ```

2. 安装singularity（[详情](https://apptainer.org/docs/admin/main/installation.html#install-from-pre-built-packages)）


   gentoo:

   ```bash
   echo "app-containers/apptainer suid" | sudo tee  /etc/portage/package.use/apptainer
   sudo emerge app-containers/apptainer
   sudo sed -i 's/# allow setuid-mount squashfs = iflimited/allow setuid-mount squashfs = yes/' /etc/apptainer/apptainer.conf
   ```

   centos:

   ```bash
   sudo yum install epel-release -y
   sudo yum install apptainer-suid -y
   sudo sed -i 's/# allow setuid-mount squashfs = iflimited/allow setuid-mount squashfs = yes/' /etc/apptainer/apptainer.conf
   ```

   ubuntu:

   ```bash
   sudo add-apt-repository -y ppa:apptainer/ppa
   sudo apt update
   sudo apt install apptainer-suid -y
   sudo sed -i 's/# allow setuid-mount squashfs = iflimited/allow setuid-mount squashfs = yes/' /etc/apptainer/apptainer.conf
   ```

  1. 运行微信

     ```bash
     singularity exec -B /run /opt/wechat.sif /opt/apps/com.qq.weixin.deepin/files/run.sh
     ```

## 本地构建最新容器镜像

1. 安装 debootstrap 和 singularity

   gentoo:

   ```bash
   echo "app-containers/apptainer suid" | sudo tee  /etc/portage/package.use/apptainer
   sudo emerge dev-util/debootstrap app-containers/apptainer
   ```

   centos:

   ```bash
   sudo yum install epel-release -y
   sudo yum install debootstrap apptainer-suid -y
   ```

   ubuntu:

   ```bash
   sudo add-apt-repository -y ppa:apptainer/ppa
   sudo apt update
   sudo apt install debootstrap apptainer-suid -y
   ```

2. 构建镜像

   ```bash
   git clone https://github.com/brighill/singularity-deepin.git
   cd singularity-deepin
   sudo singularity build /opt/wechat.sif deepin-wechat.def
   # 如果提示 > WARNING: 'nodev' mount option set on /tmp, it could be a source of failure during build process
   # 则需要设置SINGULARITY_TMPDIR为其他目录：
   # sudo mkdir /opt/tmp
   # sudo SINGULARITY_TMPDIR=/opt/tmp singularity build /opt/wechat.sif deepin-wechat.def
   # sudo rm -rf /opt/tmp
   cp -r entries/* /usr/share/
   ```

3. 运行微信

   ```bash
   singularity exec -B /run /opt/wechat.sif /opt/apps/com.qq.weixin.deepin/files/run.sh
   ```

## 常用命令

```bash
# 运行微信
singularity exec -B /run /opt/wechat.sif /opt/apps/com.qq.weixin.deepin/files/run.sh
# 唤出已运行的WeChat窗口
singularity exec /opt/wechat.sif /opt/deepinwine/tools/sendkeys.sh w wechat 4
# 微信截图
singularity exec /opt/wechat.sif /opt/deepinwine/tools/sendkeys.sh a wechat 3
# 退出微信
singularity exec /opt/wechat.sif /opt/deepinwine/tools/kill.sh wechat
```

## xdg-open-server

宿主机运行xdg-open-server,微信聊天文件和链接可以直接调用宿主机软件打开

```bash
# 编译
gcc -lX11 -lpthread main.c -o xdg-open-server
# 运行
./xdg-open-server
```
> 详情 https://github.com/kitsunyan/xdg-open-server
