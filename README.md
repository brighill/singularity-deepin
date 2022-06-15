# Singularity-wechat
## 安装 debootstrap 和 singularity 
```sh
# gentoo
sudo emerge dev-util/debootstrap app-containers/apptainer
# centos 
sudo yum install epel-release -y 
sudo yum install debootstrap singularity -y
# ubuntu
sudo apt update
sudo apt install debootstrap -y
wget https://github.com/sylabs/singularity/releases/download/v3.10.0/singularity-ce_3.10.0-$(lsb_release -cs)_amd64.deb
apt install ./singularity-ce_3.10.0-$(lsb_release -cs)_amd64.deb
```
## 本地构建镜像
```sh
git clone https://github.com/brighill/singularity-deepin.git
cd singularity-deepin
sudo singularity build /opt/wechat.sif deepin-wechat.def
# 如果提示 > WARNING: 'nodev' mount option set on /tmp, it could be a source of failure during build process
# 设置SINGULARITY_TMPDIR为其他目录：
# sudo mkdir /opt/tmp
# sudo SINGULARITY_TMPDIR=/opt/tmp singularity build /opt/wechat.sif deepin-wechat.def
# sudo rm -rf /opt/tmp
cp -r entries/* /usr/share/
```
## 常用命令
```sh
# 运行微信
singularity exec -B /run /opt/wechat.sif /opt/apps/com.qq.weixin.deepin/files/run.sh
# 唤出已运行的WeChat窗口
singularity exec /opt/wechat /opt/deepinwine/tools/sendkeys.sh w wechat 4
# 微信截图
singularity exec /opt/wechat /opt/deepinwine/tools/sendkeys.sh a wechat 3
# 退出微信
singularity exec /opt/wechat.sif /opt/deepinwine/tools/kill.sh wechat
```

## xdg-open-server 
```sh
# xdg-open-server 可以直接调用容器外部程序打开微信聊天内的文件

# 编译
gcc -lX11 -lpthread main.c -o xdg-open-server
# 运行
./xdg-open-server
```
> 详情 https://github.com/kitsunyan/xdg-open-server
