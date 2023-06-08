#!/bin/bash

# exit when any command fails
set -e

# Please make sure Tunnelblick's TAP driver is up and running
#/Applications/Tunnelblick.app/Contents/Resources/openvpnstart loadKexts 2

echo "Creating and setting up the bridge"
# Create the bridge device
ifconfig bridge1 create

# Set the IP and netmask for the bridge
ifconfig bridge1 192.168.67.1/24
ifconfig bridge1 up

# Connect bridge and tap0
ifconfig bridge1 addm tap0

cleanup() {
  echo ""
  echo "Cleaning up"
  echo "Removing bridge and shutting down tap"
  # Remove the bridge and shutdown tap
  ifconfig bridge1 destroy
  #/Applications/Tunnelblick.app/Contents/Resources/openvpnstart unloadKexts 2
  echo "Cleanup completed"
}

# Setting up signal handling
trap "exit" INT TERM ERR
trap "cleanup; kill 0" EXIT

# Starting DHCP Server
echo "************************"
echo "* Starting DHCP Server *"
echo "* Stopping with Ctrl-C *"
echo "************************"

dnsmasq --log-queries \
        --no-hosts \
        --no-resolv \
        --leasefile-ro \
        -d \
        --interface=bridge1 \
        -p0 --log-dhcp \
        --dhcp-range=192.168.67.2,192.168.67.10
