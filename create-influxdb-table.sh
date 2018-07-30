curl -i -XPOST http://localhost:8086/query --data-urlencode "q=CREATE DATABASE sensor"
curl -i -XPOST http://localhost:8086/query --data-urlencode "q=CREATE RETENTION POLICY "forever" ON "sensor" DURATION inf REPLICATION 1 DEFAULT"
