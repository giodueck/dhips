from smtplib import SMTP
import sys

import config

def send_email(to, subject, msg):
    try:
        server = SMTP('smtp.gmail.com:587')
        server.ehlo()
        server.starttls()
        server.login(config.EMAIL_ADDRESS, config.PASSWORD)
        message = "Subject: {}\n\n{}".format(subject, msg)
        server.sendmail(config.EMAIL_ADDRESS, to, message)
        server.quit()
        # print("Success: email sent.")
    except Exception as e:
        print("send_email: failed to send email: {}.".format(e))

# subject = "Test subject"
# msg = "Hello, how are you today?"
args = sys.argv;
send_email(args[1], args[2], args[3])