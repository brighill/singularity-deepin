# Singularity-wechat
## 1.安装 debootstrap 和 singularity (Gentoo)
```sh
sudo echo "sys-cluster/singularity ~amd64" >> /etc/portage/package.accept_keywords
sudo emerge dev-util/debootstrap sys-cluster/singularity
sudo cp apricot /usr/share/debootstrap/scripts
```
## 2.本地构建镜像
```sh
sudo singularity build /opt/wechat.sif deepin-wechat.def
# 如果提示 > WARNING: 'nodev' mount option set on /tmp, it could be a source of failure during build process
# 设置SINGULARITY_TMPDIR为其他目录：
# sudo mkdir /opt/tmp
# sudo SINGULARITY_TMPDIR=/opt/tmp singularity build /opt/wechat.sif deepin-wechat.def
# sudo rm -rf /opt/tmp
cp -r entries/* /usr/share/
```
## 3.运行
```sh
singularity exec /opt/wechat.sif /opt/apps/com.qq.weixin.deepin/files/run.sh
```
## 4.打开后台窗口
```sh
singularity exec /opt/wechat.sif $HOME/.deepinwine/deepin-wine-helper/sendkeys.sh w wechat 4
```
