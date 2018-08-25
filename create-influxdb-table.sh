curl -i -XPOST http://192.168.13.10:8086/query --data-urlencode "q=CREATE DATABASE sensor"
curl -i -XPOST http://192.168.13.10:8086/query --data-urlencode "q=CREATE RETENTION POLICY "forever" ON "sensor" DURATION inf REPLICATION 1 DEFAULT"
