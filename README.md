# Project for TTK4147 Autumn 2017 Group 16
By Peder and Harald

## Exercises
Take whatever you want.

## Mini project
Kok with caution.
= Test

# How to do stuff

username: root
pw: (blank)

Do so you can get in root:
1. go to 
export/nfs/etc/
2. Open shadow in nano
sudo nano shadow
3. change fist line to:
root::10933:0:99999:7:::


export/nfs is on the NGW100

# Get code over to NGW100
1. go to folder with client code
cd TTK4147-Autumn-2017-Group-16/miniProject16/client
2. compile the code
make
3. move "client" file from 'TTK4147-Autumn-2017-Group-16/miniProject16/client' to '/etc/nfs/root'
4. go to folder with the server
cd 'TTK4147-Autumn-2017-Group-16/miniProject16/server'
5. run server
./miniproject-server
6. Open minicom
7. run client on NGW100 using minicom
./client
8. plot results
cd 'TTK4147-Autumn-2017-Group-16/miniProject16/server' && gnuplot5
9. PROFIT!
