\page host-docs Hosting the Documentation

# Building

Install dependencies:

    sudo apt installcmake  # see: https://askubuntu.com/a/865294/917854
    sudo apt install build-essential
    sudo apt-get install libboost-all-dev
    sudo apt install doxygen
    sudo apt install graphviz

Now the html pages can be generated by running:

    crossing_consoles/doc/doxygen/build.sh

# Hosting

Install a webserver:

    sudo apt install apache2

Create a symlink to the build html files built by Doxygen:

    rm -r /var/www/html
    ln -s /path/to/crossing_consoles/doc/doxygen/html /var/www/html

# Automatic updating

Set up a GitHub Webhook for push events.

Place a nodejs script `github_webhook.js` outside of the repository, based
on [this tutorial](https://www.digitalocean.com/community/tutorials/how-to-use-node-js-and-github-webhooks-to-keep-remote-projects-in-sync):

    const secret = 'ur4n2YTEf6ewLGGSoEKPXHV7Uof4xPVNYvfUKzrYyWSzM3LTKs2kQKxmmjTYFCJe'
    const repo_path='/home/david/crossing_consoles/crossing_consoles'
    const build_path=repo_path+'/doc/doxygen/build.sh'
    
    const path = require('path')
    const http = require('http')
    const crypto = require('crypto')
    const exec = require('child_process').exec
    
    const hostname = '0.0.0.0'
    const port = 9911
    
    const server = http.createServer((req, res) => {
      console.log('Request:')
      req.on('data', function (chunk) {
        let sig = 'sha1=' + crypto.createHmac('sha1', secret).update(chunk.toString()).digest('hex')
    
        if (req.headers['x-hub-signature'] === sig) {
          console.log('Signature matches')
          exec('git pull', {
            cwd:repo_path
          })
          console.log('Pull done')
          exec(build_path)
          console.log('Build done')
        }
      })
    
      res.statusCode = 200
      res.setHeader('Content-Type', 'text/plain')
      res.end('Received!\n')
    })
    
    server.listen(
      port,
      hostname,
      () => { console.log(`Server running at http://${hostname}:${port}/`) })

Setup a service `cc_github_webhook.service` to automatically start the listener on boot:

    [Unit]
    Description=Github webhook for crossing consoles
    After=network.target
    
    [Service]
    Type=simple
    User=david
    ExecStart=/usr/bin/nodejs /path/to/github_webhook.js >> /tmp/cc_github_webhook.log
    Restart=on-failure
    
    [Install]
    WantedBy=multi-user.target

Install the service:

    sudo ln -s /path/to/kicker_github_webhook.service /etc/systemd/system/
    sudo systemctl enable cc_github_webhook
    sudo systemctl start cc_github_webhook
    sudo systemctl status cc_github_webhook
