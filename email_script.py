from email.message import EmailMessage
import ssl
import smtplib

email_sender = 'callmehregal@gmail.com'
email_password = 'baaa vibc crqm qqma'

email_receiver = 'ngke@oregonstate.edu'

subject = 'EMERGENCY'

body = """
EMERGENCY AT 1891 SW Campus Way
"""

em = EmailMessage()
em['From'] = email_sender
em['To'] = email_receiver
em['subject'] = subject
em.set_content(body)

context = ssl.create_default_context()

#specify server,
with smtplib.SMTP_SSL('smtp.gmail.com', 465, context=context) as smtp:
    smtp.login(email_sender, email_password)
    smtp.sendmail(email_sender, email_receiver, em.as_string())
