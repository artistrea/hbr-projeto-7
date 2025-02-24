echo "# mkdir -p /mnt/pico"
mkdir -p /mnt/pico
echo "# sudo mount /dev/$2 /mnt/pico"
sudo mount /dev/$2 /mnt/pico
echo "# ls /mnt/pico/"
ls /mnt/pico/
echo "# sudo cp $1 /mnt/pico/"
sudo cp $1 /mnt/pico/
echo "# sudo sync"
sudo sync
echo "# sudo umount /mnt/pico"
sudo umount /mnt/pico
