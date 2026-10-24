#!/bin/bash
# Run webserv and print every accept / request / response / close in a readable way.
# After each event it prints the client fds that are open, and the total fds of the process.
# Usage: ./trace.sh [config]        (default: hard.conf)
# Bodies longer than SIZE bytes are cut. Change it with: SIZE=10000 ./trace.sh

CONF="${1:-hard.conf}"
SIZE="${SIZE:-2000}"

strace -e trace=accept,recvfrom,sendto,close -e signal=none -s "$SIZE" -qq ./webserv "$CONF" 2>&1 | perl -ne '
BEGIN { $|=1 }
sub fds {
  $pid ||= `pgrep -n -x webserv`; chomp $pid;
  my $total = "?";
  if ($pid && opendir(my $d, "/proc/$pid/fd")) { $total = grep { !/^\./ } readdir($d); closedir($d) }
  my @f = sort { $a <=> $b } keys %c;
  my $list = @f ? join(" ", @f) : "none";
  print "\e[2m[open client fds: $list (" . scalar(@f) . ") | process fds right now: $total]\e[0m\n";
}
if (/^accept\(\d+,.*\) = (\d+)/) { $c{$1}=1; print "\n\e[1;32m=== NEW CLIENT (fd $1) ===\e[0m\n"; fds(); next }
if (/^(recvfrom|sendto)\((\d+), "(.*)"(\.\.\.)?, \d+,.*\) = (-?\d+)/) {
  my ($call,$fd,$s,$cut,$n)=($1,$2,$3,$4,$5);
  if ($call eq "recvfrom" && $n == 0) { print "\n\e[1;33m=== CLIENT LEFT (fd $fd) ===\e[0m\n"; next }
  next if $n < 0;
  $s =~ s/\\r\\n/\n/g; $s =~ s/\\n/\n/g; $s =~ s/\\t/\t/g; $s =~ s/\\"/"/g; $s =~ s/\\\\/\\/g;
  if ($call eq "recvfrom") { print "\n\e[1;36m--- REQUEST from fd $fd ($n bytes) ---\e[0m\n" }
  else { print "\n\e[1;35m--- RESPONSE to fd $fd ($n bytes) ---\e[0m\n" }
  print $s; print "\n[... cut]" if $cut; print "\n"; fds(); next }
if (/^close\((\d+)\)/) { if ($c{$1}) { delete $c{$1}; print "\n\e[1;31m=== CONNECTION CLOSED (fd $1) ===\e[0m\n"; fds() } next }
print unless /^[a-z_0-9]+\(/;
'
