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
    ('a2disconf', 2, true),
    ('a2dismod', 2, true),
    ('a2dissite', 2, true),
    ('a2enconf', 2, true),
    ('a2enmod', 2, true),
    ('a2ensite', 2, true),
    ('a2query', 2, true),
    ('aa-remove-unknown', 2, true),
    ('aa-status', 2, true),
    ('aa-teardown', 2, true),
    ('accessdb', 2, true),
    ('add-shell', 2, true),
    ('addgnupghome', 2, true),
    ('addgroup', 2, true),
    ('adduser', 2, true),
    ('agetty', 2, true),
    ('apache2', 2, true),
    ('apache2ctl', 2, true),
    ('apachectl', 2, true),
    ('apparmor_parser', 2, true),
    ('apparmor_status', 2, true),
    ('applygnupgdefaults', 2, true),
    ('arpd', 2, true),
    ('arptables', 2, true),
    ('arptables-nft', 2, true),
    ('arptables-nft-restore', 2, true),
    ('arptables-nft-save', 2, true),
    ('arptables-restore', 2, true),
    ('arptables-save', 2, true),
    ('atd', 2, true),
    ('badblocks', 2, true),
    ('bcache-super-show', 2, true),
    ('biosdecode', 2, true),
    ('blkdeactivate', 2, true),
    ('blkdiscard', 2, true),
    ('blkid', 2, true),
    ('blkzone', 2, true),
    ('blockdev', 2, true),
    ('bridge', 2, true),
    ('cache_check', 2, true),
    ('cache_dump', 2, true),
    ('cache_metadata_size', 2, true),
    ('cache_repair', 2, true),
    ('cache_restore', 2, true),
    ('cache_writeback', 2, true),
    ('capsh', 2, true),
    ('cfdisk', 2, true),
    ('cgdisk', 2, true),
    ('chcpu', 2, true),
    ('check_forensic', 2, true),
    ('chgpasswd', 2, true),
    ('chmem', 2, true),
    ('chpasswd', 2, true),
    ('chroot', 2, true),
    ('cpgr', 2, true),
    ('cppw', 2, true),
    ('cron', 2, true),
    ('cryptdisks_start', 2, true),
    ('cryptdisks_stop', 2, true),
    ('cryptsetup', 2, true),
    ('cryptsetup-reencrypt', 2, true),
    ('ctrlaltdel', 2, true),
    ('debugfs', 2, true),
    ('delgroup', 2, true),
    ('deluser', 2, true),
    ('depmod', 2, true),
    ('devlink', 2, true),
    ('dhclient', 2, true),
    ('dhclient-script', 2, true),
    ('dmeventd', 2, true),
    ('dmidecode', 2, true),
    ('dmsetup', 2, true),
    ('dmstats', 2, true),
    ('dosfsck', 2, true),
    ('dosfslabel', 2, true),
    ('dpkg-preconfigure', 2, true),
    ('dpkg-reconfigure', 2, true),
    ('dumpe2fs', 2, true),
    ('e2freefrag', 2, true),
    ('e2fsck', 2, true),
    ('e2image', 2, true),
    ('e2label', 2, true),
    ('e2mmpstatus', 2, true),
    ('e2scrub', 2, true),
    ('e2scrub_all', 2, true),
    ('e2undo', 2, true),
    ('e4crypt', 2, true),
    ('e4defrag', 2, true),
    ('ebtables', 2, true),
    ('ebtables-nft', 2, true),
    ('ebtables-nft-restore', 2, true),
    ('ebtables-nft-save', 2, true),
    ('ebtables-restore', 2, true),
    ('ebtables-save', 2, true),
    ('era_check', 2, true),
    ('era_dump', 2, true),
    ('era_invalidate', 2, true),
    ('era_restore', 2, true),
    ('ethtool', 2, true),
    ('faillock', 2, true),
    ('fatlabel', 2, true),
    ('fdformat', 2, true),
    ('fdisk', 2, true),
    ('filefrag', 2, true),
    ('findfs', 2, true),
    ('fixparts', 2, true),
    ('fsadm', 2, true),
    ('fsck', 2, true),
    ('fsck.btrfs', 2, true),
    ('fsck.cramfs', 2, true),
    ('fsck.ext2', 2, true),
    ('fsck.ext3', 2, true),
    ('fsck.ext4', 2, true),
    ('fsck.fat', 2, true),
    ('fsck.minix', 2, true),
    ('fsck.msdos', 2, true),
    ('fsck.vfat', 2, true),
    ('fsck.xfs', 2, true),
    ('fsfreeze', 2, true),
    ('fstab-decode', 2, true),
    ('fstrim', 2, true),
    ('gdisk', 2, true),
    ('genl', 2, true),
    ('getcap', 2, true),
    ('getpcaps', 2, true),
    ('getty', 2, true),
    ('groupadd', 2, true),
    ('groupdel', 2, true),
    ('groupmems', 2, true),
    ('groupmod', 2, true),
    ('grpck', 2, true),
    ('grpconv', 2, true),
    ('grpunconv', 2, true),
    ('halt', 2, true),
    ('hdparm', 2, true),
    ('httxt2dbm', 2, true),
    ('hwclock', 2, true),
    ('iconvconfig', 2, true),
    ('init', 2, true),
    ('insmod', 2, true),
    ('installkernel', 2, true),
    ('integritysetup', 2, true),
    ('invoke-rc.d', 2, true),
    ('ip', 2, true),
    ('ip6tables', 2, true),
    ('ip6tables-apply', 2, true),
    ('ip6tables-legacy', 2, true),
    ('ip6tables-legacy-restore', 2, true),
    ('ip6tables-legacy-save', 2, true),
    ('ip6tables-nft', 2, true),
    ('ip6tables-nft-restore', 2, true),
    ('ip6tables-nft-save', 2, true),
    ('ip6tables-restore', 2, true),
    ('ip6tables-restore-translate', 2, true),
    ('ip6tables-save', 2, true),
    ('ip6tables-translate', 2, true),
    ('iptables', 2, true),
    ('iptables-apply', 2, true),
    ('iptables-legacy', 2, true),
    ('iptables-legacy-restore', 2, true),
    ('iptables-legacy-save', 2, true),
    ('iptables-nft', 2, true),
    ('iptables-nft-restore', 2, true),
    ('iptables-nft-save', 2, true),
    ('iptables-restore', 2, true),
    ('iptables-restore-translate', 2, true),
    ('iptables-save', 2, true),
    ('iptables-translate', 2, true),
    ('irqbalance', 2, true),
    ('irqbalance-ui', 2, true),
    ('iscsi-iname', 2, true),
    ('iscsi_discovery', 2, true),
    ('iscsiadm', 2, true),
    ('iscsid', 2, true),
    ('iscsistart', 2, true),
    ('isosize', 2, true),
    ('kbdrate', 2, true),
    ('killall5', 2, true),
    ('kpartx', 2, true),
    ('ldattach', 2, true),
    ('ldconfig', 2, true),
    ('ldconfig.real', 2, true),
    ('locale-gen', 2, true),
    ('logrotate', 2, true),
    ('logsave', 2, true),
    ('losetup', 2, true),
    ('lsmod', 2, true),
    ('luksformat', 2, true),
    ('lvchange', 2, true),
    ('lvconvert', 2, true),
    ('lvcreate', 2, true),
    ('lvdisplay', 2, true),
    ('lvextend', 2, true),
    ('lvm', 2, true),
    ('lvmconfig', 2, true),
    ('lvmdiskscan', 2, true),
    ('lvmdump', 2, true),
    ('lvmpolld', 2, true),
    ('lvmsadc', 2, true),
    ('lvmsar', 2, true),
    ('lvreduce', 2, true),
    ('lvremove', 2, true),
    ('lvrename', 2, true),
    ('lvresize', 2, true),
    ('lvs', 2, true),
    ('lvscan', 2, true),
    ('make-bcache', 2, true),
    ('make-ssl-cert', 2, true),
    ('mdadm', 2, true),
    ('mdmon', 2, true),
    ('mkdosfs', 2, true),
    ('mke2fs', 2, true),
    ('mkfs', 2, true),
    ('mkfs.bfs', 2, true),
    ('mkfs.btrfs', 2, true),
    ('mkfs.cramfs', 2, true),
    ('mkfs.ext2', 2, true),
    ('mkfs.ext3', 2, true),
    ('mkfs.ext4', 2, true),
    ('mkfs.fat', 2, true),
    ('mkfs.minix', 2, true),
    ('mkfs.msdos', 2, true),
    ('mkfs.ntfs', 2, true),
    ('mkfs.vfat', 2, true),
    ('mkfs.xfs', 2, true),
    ('mkhomedir_helper', 2, true),
    ('mkinitramfs', 2, true),
    ('mklost+found', 2, true),
    ('mkntfs', 2, true),
    ('mkswap', 2, true),
    ('modinfo', 2, true),
    ('modprobe', 2, true),
    ('mount.drvfs', 2, true),
    ('mount.fuse', 2, true),
    ('mount.lowntfs-3g', 2, true),
    ('mount.ntfs', 2, true),
    ('mount.ntfs-3g', 2, true),
    ('mount.vmhgfs', 2, true),
    ('mpathpersist', 2, true),
    ('multipath', 2, true),
    ('multipathd', 2, true),
    ('netplan', 2, true),
    ('newusers', 2, true),
    ('nfnl_osf', 2, true),
    ('nologin', 2, true),
    ('ntfsclone', 2, true),
    ('ntfscp', 2, true),
    ('ntfslabel', 2, true),
    ('ntfsresize', 2, true),
    ('ntfsundelete', 2, true),
    ('on_ac_power', 2, true),
    ('overlayroot-chroot', 2, true),
    ('ownership', 2, true),
    ('pam-auth-update', 2, true),
    ('pam_extrausers_chkpwd', 2, true),
    ('pam_extrausers_update', 2, true),
    ('pam_getenv', 2, true),
    ('pam_tally', 2, true),
    ('pam_tally2', 2, true),
    ('pam_timestamp_check', 2, true),
    ('parted', 2, true),
    ('partprobe', 2, true),
    ('pdata_tools', 2, true),
    ('pg_updatedicts', 2, true),
    ('pivot_root', 2, true),
    ('plymouthd', 2, true),
    ('popcon-largest-unused', 2, true),
    ('popularity-contest', 2, true),
    ('poweroff', 2, true),
    ('pvchange', 2, true),
    ('pvck', 2, true),
    ('pvcreate', 2, true),
    ('pvdisplay', 2, true),
    ('pvmove', 2, true),
    ('pvremove', 2, true),
    ('pvresize', 2, true),
    ('pvs', 2, true),
    ('pvscan', 2, true),
    ('pwck', 2, true),
    ('pwconv', 2, true),
    ('pwunconv', 2, true),
    ('raw', 2, true),
    ('readprofile', 2, true),
    ('reboot', 2, true),
    ('remove-shell', 2, true),
    ('resize2fs', 2, true),
    ('rmmod', 2, true),
    ('rmt', 2, true),
    ('rmt-tar', 2, true),
    ('rsyslogd', 2, true),
    ('rtacct', 2, true),
    ('rtcwake', 2, true),
    ('rtmon', 2, true),
    ('runlevel', 2, true),
    ('runuser', 2, true),
    ('service', 2, true),
    ('setcap', 2, true),
    ('setvesablank', 2, true),
    ('setvtrgb', 2, true),
    ('sfdisk', 2, true),
    ('sgdisk', 2, true),
    ('shadowconfig', 2, true),
    ('shutdown', 2, true),
    ('split-logfile', 2, true),
    ('sshd', 2, true),
    ('start-stop-daemon', 2, true),
    ('sulogin', 2, true),
    ('swaplabel', 2, true),
    ('swapoff', 2, true),
    ('swapon', 2, true),
    ('switch_root', 2, true),
    ('sysctl', 2, true),
    ('tarcat', 2, true),
    ('tc', 2, true),
    ('tcpdump', 2, true),
    ('telinit', 2, true),
    ('thin_check', 2, true),
    ('thin_delta', 2, true),
    ('thin_dump', 2, true),
    ('thin_ls', 2, true),
    ('thin_metadata_size', 2, true),
    ('thin_repair', 2, true),
    ('thin_restore', 2, true),
    ('thin_rmap', 2, true),
    ('thin_trim', 2, true),
    ('tipc', 2, true),
    ('tune2fs', 2, true),
    ('tzconfig', 2, true),
    ('ufw', 2, true),
    ('unix_chkpwd', 2, true),
    ('unix_update', 2, true),
    ('update-ca-certificates', 2, true),
    ('update-info-dir', 2, true),
    ('update-initramfs', 2, true),
    ('update-locale', 2, true),
    ('update-mime', 2, true),
    ('update-motd', 2, true),
    ('update-passwd', 2, true),
    ('update-pciids', 2, true),
    ('update-rc.d', 2, true),
    ('useradd', 2, true),
    ('userdel', 2, true),
    ('usermod', 2, true),
    ('uuidd', 2, true),
    ('validlocale', 2, true),
    ('vcstime', 2, true),
    ('veritysetup', 2, true),
    ('vgcfgbackup', 2, true),
    ('vgcfgrestore', 2, true),
    ('vgchange', 2, true),
    ('vgck', 2, true),
    ('vgconvert', 2, true),
    ('vgcreate', 2, true),
    ('vgdisplay', 2, true),
    ('vgexport', 2, true),
    ('vgextend', 2, true),
    ('vgimport', 2, true),
    ('vgimportclone', 2, true),
    ('vgmerge', 2, true),
    ('vgmknodes', 2, true),
    ('vgreduce', 2, true),
    ('vgremove', 2, true),
    ('vgrename', 2, true),
    ('vgs', 2, true),
    ('vgscan', 2, true),
    ('vgsplit', 2, true),
    ('vigr', 2, true),
    ('vipw', 2, true),
    ('visudo', 2, true),
    ('vpddecode', 2, true),
    ('wipefs', 2, true),
    ('xfs_admin', 2, true),
    ('xfs_bmap', 2, true),
    ('xfs_copy', 2, true),
    ('xfs_db', 2, true),
    ('xfs_estimate', 2, true),
    ('xfs_freeze', 2, true),
    ('xfs_fsr', 2, true),
    ('xfs_growfs', 2, true),
    ('xfs_info', 2, true),
    ('xfs_io', 2, true),
    ('xfs_logprint', 2, true),
    ('xfs_mdrestore', 2, true),
    ('xfs_metadump', 2, true),
    ('xfs_mkfile', 2, true),
    ('xfs_ncheck', 2, true),
    ('xfs_quota', 2, true),
    ('xfs_repair', 2, true),
    ('xfs_rtcp', 2, true),
    ('xfs_scrub', 2, true),
    ('xfs_scrub_all', 2, true),
    ('xfs_spaceman', 2, true),
    ('xtables-legacy-multi', 2, true),
    ('xtables-monitor', 2, true),
    ('xtables-nft-multi', 2, true),
    ('zerofree', 2, true),
    ('zic', 2, true),
    ('zramctl', 2, true);