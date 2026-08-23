
HTTP request response structure:

<Method> <Request-URI> <HTTP-Version>
<Header-Name>: <Header-Value>
...
<Header-Name>: <Header-Value>

<Optional-Body>

example:
GET /index.html HTTP/1.1
Host: www.example.com
User-Agent: Mozilla/5.0
Accept: text/html

how to detect termination:

Three different ways:

content-length header (number of bytes following the headers)

chunked encoding (content length unknown at start of request, chunked encoding will indicate when the end is reached)

connection closed by server (http "0.9")

Here is better explanation
https://stackoverflow.com/questions/77445815/efficient-ways-to-parse-an-http-request-in-c