const http = require('http');
const fs = require("fs");
const URL = require("url");
const checkuser = require('./build/Release/checkuser.node')

const hostname = '127.0.0.1';
const port = 7788;

const server = http.createServer( (req, res) => {
   const requestURL = URL.parse(req.url, true);
   const method = requestURL.path.split("?")[0];
   const username = requestURL.query.username;

   if (req.method === 'GET' && method === '/user') {
      const info = checkuser.getLocalGroups(username);

      if(info.length === 0) {
         res.writeHead(200, { 'Content-Type' : 'text/html; charset=utf-8' });
         res.write(`Пользователя ${username} нет<br><a href="/">Назад</a>`);
         return res.end();
      } else {
         let foundGroup;
         info.forEach((group) => {
            switch(group) {
               case 'Администраторы' || 'Administrators':
                  foundGroup = "Администратор";
                  break;
               case 'Пользователи' || 'Users':
                  foundGroup = "Пользователь";
                  break;
               case 'Гости' || 'Guests':
                  foundGroup = "Гость";
                  break;
            }
         })
         res.writeHead(200, { 'Content-Type' : 'text/html; charset=utf-8' });
         res.write(`Пользователь ${username} имеет привилегию ${foundGroup}<br><a href="/">Назад</a>`);
         return res.end();
      }
   } else {
      fs.readFile('./index.html', (_, data) => {
         res.writeHead(200, { 'Content-Type' : 'text/html'});
         res.write(data);
         return res.end();
      })
   }
});

server.listen(port, hostname, () => {

});