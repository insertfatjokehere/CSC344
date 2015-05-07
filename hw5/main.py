#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import zipfile
import smtplib
import getpass
from email.MIMEMultipart import MIMEMultipart
from email.MIMEBase import MIMEBase
from email.MIMEText import MIMEText
from email import encoders

def main():
	if (sys.platform.startswith('win')):
		os.system('cls')
	else:
		os.system('clear')
	print("\n=== HW5 Manager ===")
	os.chdir(os.pardir)
	cscdir = os.getcwd()
	getFiles(cscdir)
	zipProject(cscdir)
	mailProject()
	print("\n=== Exiting ===\n")
	return 0


def getFiles(dir):
	print("\n--- Working Directory ---")
	print(dir + "\n")
	subdir = os.listdir(dir)
	subdir.sort()
	print("--- Sub Directories ---")
	# generate html after generating directory list

	html = open('index.html', 'w')
	html.write('<!DOCTYPE html>\n<html>\n<head>\n<title>CSC344</title>\n</head>\n<body>\n<h3>Directory of CSC344</h3>\n')
	html.write('<ul>\n')
	
	for sub in subdir:
		# this is used so incase index.html exists, dont look at it.
		if (os.path.isdir(sub)):
			html.write('<li> folder: <a href=\"' + sub + '\">' + sub + '</a>\n')
			print("\n- folder: " + sub)
			readFiles(sub, html)
		

	html.write('</body>\n</html>')
	html.close()
	print("\n--- Created " + html.name + " ---")

def readFiles(folder, html):
	files = os.listdir(folder)
	html.write('<ul>\n')
	for fil in files:
		html.write('<li> <a href=\"'+ folder + '/' + fil + '\">' + fil + '</a> ')
		print(" |- file: " + fil)
		total = lineCount(folder, fil)
		html.write(str(total)+ ' lines</li>\n')

	html.write('</ul>\n</li>\n')


def lineCount(folder, fil):
	pathFile = folder + '/' + fil
	total_line_count = 0
	isComment = False
	# sum(1 for line in open('myfile.txt'))
	extension = os.path.splitext(pathFile)[1]
	openedFile = open(pathFile)

	if extension == '.c' or extension == '.scala' or extension == '.pl':
		for line in openedFile:
			line = line.strip().replace(" ","").replace("\t","")
			if (line.startswith('/*') and line.endswith('*/')) or line.startswith('//') or line == "":
				# do nothing: comment lines or empty lines
				pass
			elif line.startswith('/*'):
				# start of comment
				isComment = True
			elif isComment == True and not line.endswith('*/'):
				# do nothing: comment lines or empty lines
				pass
			elif line.endswith('*/') and isComment == True:
				# end of comment
				isComment = False
			else:
				total_line_count += 1
	elif extension == '.lisp':
		for line in openedFile:
			line = line.strip().replace(" ","").replace("\t","")
			if line.startswith(';') or line == "":
				# do nothing: comment lines or empty lines
				pass
			else:
				total_line_count += 1
	elif extension == '.py':
		for line in openedFile:
			line = line.strip().replace(" ","").replace("\t","")
			if line.startswith('#') or line == "":
				# do nothing: comment lines or empty lines
				pass
			else:
				total_line_count += 1
	else:
		total_line_count = sum(1 for line in open(pathFile))

	return total_line_count

def zipProject(dir):
	if os.path.isfile("CSC344.zip"):
		os.remove("CSC344.zip")
	# need to call this again, after making html
	subdir = os.listdir(dir)
	zipper = zipfile.ZipFile("CSC344.zip", "w")
	print("\n--- Zipping File: CSC344.zip ---\n")
	for subs in subdir:
		if (os.path.isdir(subs)):
			files = os.listdir(subs)
			for fil in files:
				print("compressing " + subs+"/"+fil)
				zipper.write(subs+"/"+fil)
		else:
			print("compressing " + subs)
			zipper.write(subs)
	zipper.close

def mailProject():
	try:
		msg = MIMEMultipart()
		print("\n--- Send Email ---\n")

		user = raw_input('Enter GMAIL username >>> ')
		pw = getpass.getpass('Enter Password >>> ')
		reciever = raw_input('Enter the email address >>> ')

		msg['From'] = user
   		msg['To'] = reciever
  		msg['Subject'] = "CSC344 HW5 Submission"

  		msg.attach(MIMEText("View Attachment For Submission"))
  		zipper = "CSC344.zip"
		part = MIMEBase('application', 'octet-stream')
		part.set_payload(open(zipper, 'rb').read())
		encoders.encode_base64(part)
		part.add_header('Content-Disposition','attachment; filename="%s"' % os.path.basename(zipper))
		msg.attach(part)

		mailServer = smtplib.SMTP("smtp.gmail.com", 587)
		mailServer.ehlo()
		mailServer.starttls()
		mailServer.ehlo()
		mailServer.login(user, pw)
		mailServer.sendmail(user, reciever, msg.as_string())
		mailServer.close()
		print "\nEmail successful"
	except Exception:
		print "\nCannot send email"

main()