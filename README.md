repository status: adding code, no process yet

# Ammit
meta meta initramfs linux distribution

Created from a personal need, provides genuine features different from other distributions. No warranty provided, user is responsible for checking fitness for purpose including security. Named Ammit because Phoenix is overused but it is a very good description of what this distribution is. Free for anyone to use.

A linux distribution based on the Gentoo "meta" distribution for those without time to waste. Ammit goes a step further to abstract the system from the data and specific wanted configurations.
It achieves this by modifying what is basically a user modified Gentoo stage 3 tarball which has had extra packages and emerge configurations installed. Everything else (software configuration) is handled by the initramfs. This allows some cool things that save lots of time for the admin.
 * multiple software configurations can be paired with the same system configurations
 * the system is a fixed image so any issues can be solved by using a previous version
 * initramfs can detect machine IDs so the same system can be deployed on many machines with slight changes in configuration such as ssh keys and hostname
 * system images can be tested on a test machine before being deployed to others
 * based on Gentoo so no painful version upgrades, system can be completely changed using a few emerege configuration files
 * can run from a USB key, can run from RAM
 * allows no mess from being saved in your home directory
