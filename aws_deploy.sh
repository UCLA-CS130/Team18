docker save -o httpserver.tar httpserver
scp -l 8192 -i key1.pem httpserver.tar ubuntu@ec2-35-166-129-63.us-west-2.compute.amazonaws.com:~/httpserver.tar
ssh -i key1.pem ubuntu@ec2-35-166-129-63.us-west-2.compute.amazonaws.com << ENDSSH 
docker load -i httpserver.tar
docker run --rm -t -p 8080:8080 httpserver
ENDSSH

