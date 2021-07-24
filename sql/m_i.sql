-- System files
insert into monitor (fullfilename, type, active) values
    ('/etc/passwd', 1, true),
    ('/etc/shadow', 1, true),
    ('/etc/aliases', 1, true),
    ('/etc/bashrc', 1, true),
    ('/etc/crontab', 1, true),
    ('/etc/exports', 1, true),
    ('/etc/fstab', 1, true),
    ('/etc/group', 1, true),
    ('/etc/grub.conf', 1, true),
    ('/etc/hosts', 1, true),
    ('/etc/hosts.allow', 1, true),
    ('/etc/hosts.deny', 1, true),
    ('/etc/inittab', 1, true),
    ('/etc/issue', 1, true),
    ('/etc/modules.conf', 1, true),
    ('/etc/motd', 1, true),
    ('/etc/mtab', 1, true),
    ('/etc/profile', 1, true),
    ('/etc/resolv.conf', 1, true),
    ('/etc/securetty', 1, true),
    ('/etc/termcap', 1, true);

-- Binaries
insert into monitor (fullfilename, type, active) values
    ('/sbin/a2disconf', 2, true),
    ('/sbin/a2dismod', 2, true),
    ('/sbin/a2dissite', 2, true),
    ('/sbin/a2enconf', 2, true),
    ('/sbin/a2enmod', 2, true),
    ('/sbin/a2ensite', 2, true),
    ('/sbin/a2query', 2, true),
    ('/sbin/aa-remove-unknown', 2, true),
    ('/sbin/aa-status', 2, true),
    ('/sbin/aa-teardown', 2, true),
    ('/sbin/accessdb', 2, true),
    ('/sbin/add-shell', 2, true),
    ('/sbin/addgnupghome', 2, true),
    ('/sbin/addgroup', 2, true),
    ('/sbin/adduser', 2, true),
    ('/sbin/agetty', 2, true),
    ('/sbin/apache2', 2, true),
    ('/sbin/apache2ctl', 2, true),
    ('/sbin/apachectl', 2, true),
    ('/sbin/apparmor_parser', 2, true),
    ('/sbin/apparmor_status', 2, true),
    ('/sbin/applygnupgdefaults', 2, true),
    ('/sbin/arpd', 2, true),
    ('/sbin/arptables', 2, true),
    ('/sbin/arptables-nft', 2, true),
    ('/sbin/arptables-nft-restore', 2, true),
    ('/sbin/arptables-nft-save', 2, true),
    ('/sbin/arptables-restore', 2, true),
    ('/sbin/arptables-save', 2, true),
    ('/sbin/atd', 2, true),
    ('/sbin/badblocks', 2, true),
    ('/sbin/bcache-super-show', 2, true),
    ('/sbin/biosdecode', 2, true),
    ('/sbin/blkdeactivate', 2, true),
    ('/sbin/blkdiscard', 2, true),
    ('/sbin/blkid', 2, true),
    ('/sbin/blkzone', 2, true),
    ('/sbin/blockdev', 2, true),
    ('/sbin/bridge', 2, true),
    ('/sbin/cache_check', 2, true),
    ('/sbin/cache_dump', 2, true),
    ('/sbin/cache_metadata_size', 2, true),
    ('/sbin/cache_repair', 2, true),
    ('/sbin/cache_restore', 2, true),
    ('/sbin/cache_writeback', 2, true),
    ('/sbin/capsh', 2, true),
    ('/sbin/cfdisk', 2, true),
    ('/sbin/cgdisk', 2, true),
    ('/sbin/chcpu', 2, true),
    ('/sbin/check_forensic', 2, true),
    ('/sbin/chgpasswd', 2, true),
    ('/sbin/chmem', 2, true),
    ('/sbin/chpasswd', 2, true),
    ('/sbin/chroot', 2, true),
    ('/sbin/cpgr', 2, true),
    ('/sbin/cppw', 2, true),
    ('/sbin/cron', 2, true),
    ('/sbin/cryptdisks_start', 2, true),
    ('/sbin/cryptdisks_stop', 2, true),
    ('/sbin/cryptsetup', 2, true),
    ('/sbin/cryptsetup-reencrypt', 2, true),
    ('/sbin/ctrlaltdel', 2, true),
    ('/sbin/debugfs', 2, true),
    ('/sbin/delgroup', 2, true),
    ('/sbin/deluser', 2, true),
    ('/sbin/depmod', 2, true),
    ('/sbin/devlink', 2, true),
    ('/sbin/dhclient', 2, true),
    ('/sbin/dhclient-script', 2, true),
    ('/sbin/dmeventd', 2, true),
    ('/sbin/dmidecode', 2, true),
    ('/sbin/dmsetup', 2, true),
    ('/sbin/dmstats', 2, true),
    ('/sbin/dosfsck', 2, true),
    ('/sbin/dosfslabel', 2, true),
    ('/sbin/dpkg-preconfigure', 2, true),
    ('/sbin/dpkg-reconfigure', 2, true),
    ('/sbin/dumpe2fs', 2, true),
    ('/sbin/e2freefrag', 2, true),
    ('/sbin/e2fsck', 2, true),
    ('/sbin/e2image', 2, true),
    ('/sbin/e2label', 2, true),
    ('/sbin/e2mmpstatus', 2, true),
    ('/sbin/e2scrub', 2, true),
    ('/sbin/e2scrub_all', 2, true),
    ('/sbin/e2undo', 2, true),
    ('/sbin/e4crypt', 2, true),
    ('/sbin/e4defrag', 2, true),
    ('/sbin/ebtables', 2, true),
    ('/sbin/ebtables-nft', 2, true),
    ('/sbin/ebtables-nft-restore', 2, true),
    ('/sbin/ebtables-nft-save', 2, true),
    ('/sbin/ebtables-restore', 2, true),
    ('/sbin/ebtables-save', 2, true),
    ('/sbin/era_check', 2, true),
    ('/sbin/era_dump', 2, true),
    ('/sbin/era_invalidate', 2, true),
    ('/sbin/era_restore', 2, true),
    ('/sbin/ethtool', 2, true),
    ('/sbin/faillock', 2, true),
    ('/sbin/fatlabel', 2, true),
    ('/sbin/fdformat', 2, true),
    ('/sbin/fdisk', 2, true),
    ('/sbin/filefrag', 2, true),
    ('/sbin/findfs', 2, true),
    ('/sbin/fixparts', 2, true),
    ('/sbin/fsadm', 2, true),
    ('/sbin/fsck', 2, true),
    ('/sbin/fsck.btrfs', 2, true),
    ('/sbin/fsck.cramfs', 2, true),
    ('/sbin/fsck.ext2', 2, true),
    ('/sbin/fsck.ext3', 2, true),
    ('/sbin/fsck.ext4', 2, true),
    ('/sbin/fsck.fat', 2, true),
    ('/sbin/fsck.minix', 2, true),
    ('/sbin/fsck.msdos', 2, true),
    ('/sbin/fsck.vfat', 2, true),
    ('/sbin/fsck.xfs', 2, true),
    ('/sbin/fsfreeze', 2, true),
    ('/sbin/fstab-decode', 2, true),
    ('/sbin/fstrim', 2, true),
    ('/sbin/gdisk', 2, true),
    ('/sbin/genl', 2, true),
    ('/sbin/getcap', 2, true),
    ('/sbin/getpcaps', 2, true),
    ('/sbin/getty', 2, true),
    ('/sbin/groupadd', 2, true),
    ('/sbin/groupdel', 2, true),
    ('/sbin/groupmems', 2, true),
    ('/sbin/groupmod', 2, true),
    ('/sbin/grpck', 2, true),
    ('/sbin/grpconv', 2, true),
    ('/sbin/grpunconv', 2, true),
    ('/sbin/halt', 2, true),
    ('/sbin/hdparm', 2, true),
    ('/sbin/httxt2dbm', 2, true),
    ('/sbin/hwclock', 2, true),
    ('/sbin/iconvconfig', 2, true),
    ('/sbin/init', 2, true),
    ('/sbin/insmod', 2, true),
    ('/sbin/installkernel', 2, true),
    ('/sbin/integritysetup', 2, true),
    ('/sbin/invoke-rc.d', 2, true),
    ('/sbin/ip', 2, true),
    ('/sbin/ip6tables', 2, true),
    ('/sbin/ip6tables-apply', 2, true),
    ('/sbin/ip6tables-legacy', 2, true),
    ('/sbin/ip6tables-legacy-restore', 2, true),
    ('/sbin/ip6tables-legacy-save', 2, true),
    ('/sbin/ip6tables-nft', 2, true),
    ('/sbin/ip6tables-nft-restore', 2, true),
    ('/sbin/ip6tables-nft-save', 2, true),
    ('/sbin/ip6tables-restore', 2, true),
    ('/sbin/ip6tables-restore-translate', 2, true),
    ('/sbin/ip6tables-save', 2, true),
    ('/sbin/ip6tables-translate', 2, true),
    ('/sbin/iptables', 2, true),
    ('/sbin/iptables-apply', 2, true),
    ('/sbin/iptables-legacy', 2, true),
    ('/sbin/iptables-legacy-restore', 2, true),
    ('/sbin/iptables-legacy-save', 2, true),
    ('/sbin/iptables-nft', 2, true),
    ('/sbin/iptables-nft-restore', 2, true),
    ('/sbin/iptables-nft-save', 2, true),
    ('/sbin/iptables-restore', 2, true),
    ('/sbin/iptables-restore-translate', 2, true),
    ('/sbin/iptables-save', 2, true),
    ('/sbin/iptables-translate', 2, true),
    ('/sbin/irqbalance', 2, true),
    ('/sbin/irqbalance-ui', 2, true),
    ('/sbin/iscsi-iname', 2, true),
    ('/sbin/iscsi_discovery', 2, true),
    ('/sbin/iscsiadm', 2, true),
    ('/sbin/iscsid', 2, true),
    ('/sbin/iscsistart', 2, true),
    ('/sbin/isosize', 2, true),
    ('/sbin/kbdrate', 2, true),
    ('/sbin/killall5', 2, true),
    ('/sbin/kpartx', 2, true),
    ('/sbin/ldattach', 2, true),
    ('/sbin/ldconfig', 2, true),
    ('/sbin/ldconfig.real', 2, true),
    ('/sbin/locale-gen', 2, true),
    ('/sbin/logrotate', 2, true),
    ('/sbin/logsave', 2, true),
    ('/sbin/losetup', 2, true),
    ('/sbin/lsmod', 2, true),
    ('/sbin/luksformat', 2, true),
    ('/sbin/lvchange', 2, true),
    ('/sbin/lvconvert', 2, true),
    ('/sbin/lvcreate', 2, true),
    ('/sbin/lvdisplay', 2, true),
    ('/sbin/lvextend', 2, true),
    ('/sbin/lvm', 2, true),
    ('/sbin/lvmconfig', 2, true),
    ('/sbin/lvmdiskscan', 2, true),
    ('/sbin/lvmdump', 2, true),
    ('/sbin/lvmpolld', 2, true),
    ('/sbin/lvmsadc', 2, true),
    ('/sbin/lvmsar', 2, true),
    ('/sbin/lvreduce', 2, true),
    ('/sbin/lvremove', 2, true),
    ('/sbin/lvrename', 2, true),
    ('/sbin/lvresize', 2, true),
    ('/sbin/lvs', 2, true),
    ('/sbin/lvscan', 2, true),
    ('/sbin/make-bcache', 2, true),
    ('/sbin/make-ssl-cert', 2, true),
    ('/sbin/mdadm', 2, true),
    ('/sbin/mdmon', 2, true),
    ('/sbin/mkdosfs', 2, true),
    ('/sbin/mke2fs', 2, true),
    ('/sbin/mkfs', 2, true),
    ('/sbin/mkfs.bfs', 2, true),
    ('/sbin/mkfs.btrfs', 2, true),
    ('/sbin/mkfs.cramfs', 2, true),
    ('/sbin/mkfs.ext2', 2, true),
    ('/sbin/mkfs.ext3', 2, true),
    ('/sbin/mkfs.ext4', 2, true),
    ('/sbin/mkfs.fat', 2, true),
    ('/sbin/mkfs.minix', 2, true),
    ('/sbin/mkfs.msdos', 2, true),
    ('/sbin/mkfs.ntfs', 2, true),
    ('/sbin/mkfs.vfat', 2, true),
    ('/sbin/mkfs.xfs', 2, true),
    ('/sbin/mkhomedir_helper', 2, true),
    ('/sbin/mkinitramfs', 2, true),
    ('/sbin/mklost+found', 2, true),
    ('/sbin/mkntfs', 2, true),
    ('/sbin/mkswap', 2, true),
    ('/sbin/modinfo', 2, true),
    ('/sbin/modprobe', 2, true),
    ('/sbin/mount.drvfs', 2, true),
    ('/sbin/mount.fuse', 2, true),
    ('/sbin/mount.lowntfs-3g', 2, true),
    ('/sbin/mount.ntfs', 2, true),
    ('/sbin/mount.ntfs-3g', 2, true),
    ('/sbin/mount.vmhgfs', 2, true),
    ('/sbin/mpathpersist', 2, true),
    ('/sbin/multipath', 2, true),
    ('/sbin/multipathd', 2, true),
    ('/sbin/netplan', 2, true),
    ('/sbin/newusers', 2, true),
    ('/sbin/nfnl_osf', 2, true),
    ('/sbin/nologin', 2, true),
    ('/sbin/ntfsclone', 2, true),
    ('/sbin/ntfscp', 2, true),
    ('/sbin/ntfslabel', 2, true),
    ('/sbin/ntfsresize', 2, true),
    ('/sbin/ntfsundelete', 2, true),
    ('/sbin/on_ac_power', 2, true),
    ('/sbin/overlayroot-chroot', 2, true),
    ('/sbin/ownership', 2, true),
    ('/sbin/pam-auth-update', 2, true),
    ('/sbin/pam_extrausers_chkpwd', 2, true),
    ('/sbin/pam_extrausers_update', 2, true),
    ('/sbin/pam_getenv', 2, true),
    ('/sbin/pam_tally', 2, true),
    ('/sbin/pam_tally2', 2, true),
    ('/sbin/pam_timestamp_check', 2, true),
    ('/sbin/parted', 2, true),
    ('/sbin/partprobe', 2, true),
    ('/sbin/pdata_tools', 2, true),
    ('/sbin/pg_updatedicts', 2, true),
    ('/sbin/pivot_root', 2, true),
    ('/sbin/plymouthd', 2, true),
    ('/sbin/popcon-largest-unused', 2, true),
    ('/sbin/popularity-contest', 2, true),
    ('/sbin/poweroff', 2, true),
    ('/sbin/pvchange', 2, true),
    ('/sbin/pvck', 2, true),
    ('/sbin/pvcreate', 2, true),
    ('/sbin/pvdisplay', 2, true),
    ('/sbin/pvmove', 2, true),
    ('/sbin/pvremove', 2, true),
    ('/sbin/pvresize', 2, true),
    ('/sbin/pvs', 2, true),
    ('/sbin/pvscan', 2, true),
    ('/sbin/pwck', 2, true),
    ('/sbin/pwconv', 2, true),
    ('/sbin/pwunconv', 2, true),
    ('/sbin/raw', 2, true),
    ('/sbin/readprofile', 2, true),
    ('/sbin/reboot', 2, true),
    ('/sbin/remove-shell', 2, true),
    ('/sbin/resize2fs', 2, true),
    ('/sbin/rmmod', 2, true),
    ('/sbin/rmt', 2, true),
    ('/sbin/rmt-tar', 2, true),
    ('/sbin/rsyslogd', 2, true),
    ('/sbin/rtacct', 2, true),
    ('/sbin/rtcwake', 2, true),
    ('/sbin/rtmon', 2, true),
    ('/sbin/runlevel', 2, true),
    ('/sbin/runuser', 2, true),
    ('/sbin/service', 2, true),
    ('/sbin/setcap', 2, true),
    ('/sbin/setvesablank', 2, true),
    ('/sbin/setvtrgb', 2, true),
    ('/sbin/sfdisk', 2, true),
    ('/sbin/sgdisk', 2, true),
    ('/sbin/shadowconfig', 2, true),
    ('/sbin/shutdown', 2, true),
    ('/sbin/split-logfile', 2, true),
    ('/sbin/sshd', 2, true),
    ('/sbin/start-stop-daemon', 2, true),
    ('/sbin/sulogin', 2, true),
    ('/sbin/swaplabel', 2, true),
    ('/sbin/swapoff', 2, true),
    ('/sbin/swapon', 2, true),
    ('/sbin/switch_root', 2, true),
    ('/sbin/sysctl', 2, true),
    ('/sbin/tarcat', 2, true),
    ('/sbin/tc', 2, true),
    ('/sbin/tcpdump', 2, true),
    ('/sbin/telinit', 2, true),
    ('/sbin/thin_check', 2, true),
    ('/sbin/thin_delta', 2, true),
    ('/sbin/thin_dump', 2, true),
    ('/sbin/thin_ls', 2, true),
    ('/sbin/thin_metadata_size', 2, true),
    ('/sbin/thin_repair', 2, true),
    ('/sbin/thin_restore', 2, true),
    ('/sbin/thin_rmap', 2, true),
    ('/sbin/thin_trim', 2, true),
    ('/sbin/tipc', 2, true),
    ('/sbin/tune2fs', 2, true),
    ('/sbin/tzconfig', 2, true),
    ('/sbin/ufw', 2, true),
    ('/sbin/unix_chkpwd', 2, true),
    ('/sbin/unix_update', 2, true),
    ('/sbin/update-ca-certificates', 2, true),
    ('/sbin/update-info-dir', 2, true),
    ('/sbin/update-initramfs', 2, true),
    ('/sbin/update-locale', 2, true),
    ('/sbin/update-mime', 2, true),
    ('/sbin/update-motd', 2, true),
    ('/sbin/update-passwd', 2, true),
    ('/sbin/update-pciids', 2, true),
    ('/sbin/update-rc.d', 2, true),
    ('/sbin/useradd', 2, true),
    ('/sbin/userdel', 2, true),
    ('/sbin/usermod', 2, true),
    ('/sbin/uuidd', 2, true),
    ('/sbin/validlocale', 2, true),
    ('/sbin/vcstime', 2, true),
    ('/sbin/veritysetup', 2, true),
    ('/sbin/vgcfgbackup', 2, true),
    ('/sbin/vgcfgrestore', 2, true),
    ('/sbin/vgchange', 2, true),
    ('/sbin/vgck', 2, true),
    ('/sbin/vgconvert', 2, true),
    ('/sbin/vgcreate', 2, true),
    ('/sbin/vgdisplay', 2, true),
    ('/sbin/vgexport', 2, true),
    ('/sbin/vgextend', 2, true),
    ('/sbin/vgimport', 2, true),
    ('/sbin/vgimportclone', 2, true),
    ('/sbin/vgmerge', 2, true),
    ('/sbin/vgmknodes', 2, true),
    ('/sbin/vgreduce', 2, true),
    ('/sbin/vgremove', 2, true),
    ('/sbin/vgrename', 2, true),
    ('/sbin/vgs', 2, true),
    ('/sbin/vgscan', 2, true),
    ('/sbin/vgsplit', 2, true),
    ('/sbin/vigr', 2, true),
    ('/sbin/vipw', 2, true),
    ('/sbin/visudo', 2, true),
    ('/sbin/vpddecode', 2, true),
    ('/sbin/wipefs', 2, true),
    ('/sbin/xfs_admin', 2, true),
    ('/sbin/xfs_bmap', 2, true),
    ('/sbin/xfs_copy', 2, true),
    ('/sbin/xfs_db', 2, true),
    ('/sbin/xfs_estimate', 2, true),
    ('/sbin/xfs_freeze', 2, true),
    ('/sbin/xfs_fsr', 2, true),
    ('/sbin/xfs_growfs', 2, true),
    ('/sbin/xfs_info', 2, true),
    ('/sbin/xfs_io', 2, true),
    ('/sbin/xfs_logprint', 2, true),
    ('/sbin/xfs_mdrestore', 2, true),
    ('/sbin/xfs_metadump', 2, true),
    ('/sbin/xfs_mkfile', 2, true),
    ('/sbin/xfs_ncheck', 2, true),
    ('/sbin/xfs_quota', 2, true),
    ('/sbin/xfs_repair', 2, true),
    ('/sbin/xfs_rtcp', 2, true),
    ('/sbin/xfs_scrub', 2, true),
    ('/sbin/xfs_scrub_all', 2, true),
    ('/sbin/xfs_spaceman', 2, true),
    ('/sbin/xtables-legacy-multi', 2, true),
    ('/sbin/xtables-monitor', 2, true),
    ('/sbin/xtables-nft-multi', 2, true),
    ('/sbin/zerofree', 2, true),
    ('/sbin/zic', 2, true),
    ('/sbin/zramctl', 2, true);