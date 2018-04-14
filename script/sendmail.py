#!/usr/bin/python

import os
import sys
import smtplib
import email.utils
from email.mime.text import MIMEText


def send_mail(to, subject, msg):
    try:
        mail_server = os.getenv('MAIL_SERVER')
        mail_user = os.getenv('MAIL_USER')
        mail_password = os.getenv('MAIL_PASSWORD')
        server = smtplib.SMTP(mail_server)
        #server.set_debuglevel(1)
        server.login(mail_user, mail_password)
        fromaddr = mail_user
        toaddrs = [to]
        #msg_header = ("From: %s\r\nTo: %s\r\n" % (fromaddr, ", ".join(toaddrs)))
        #msg = msg_header + msg
        mail_msg = MIMEText(msg)
        mail_msg['To'] = email.utils.formataddr((to.split('@')[0], to))
        mail_msg['From'] = email.utils.formataddr(('RaspberryPi', fromaddr))
        mail_msg['Subject'] = subject
        server.sendmail(fromaddr, toaddrs, mail_msg.as_string())
        res = True
    except:
        sys.stderr.write('send mail to ' + to + ' fail: '+ str(sys.exc_info()[0]) + '\n')
        res = False
    finally:
        server.quit()
        return res

if __name__ == "__main__":
    import getopt
    ok = True
    do_help = False
    subject = ''
    try:
        optlist, args = getopt.getopt(sys.argv[1:], 's:')
    except getopt.error:
        ok = False
    if not ok or len(args) == 0:
        do_help = True
    for o, a in optlist:
        if o == '-s': subject = a
    if do_help:
        sys.stderr.write('Usage: '+ sys.argv[0] + ' <-s subject> <to>...\n')
        sys.exit(1)
    msg = ''
    while True:
        try:
            line = sys.stdin.readline()
        except EOFError:
            break
        if not line: break
        msg = msg + line
    for to in args:
        send_mail(to, subject, msg)
