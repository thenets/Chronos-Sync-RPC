#!/bin/python
from gi.repository import Gtk

import sys
import os
import subprocess
import time
import thread



# Check root access
# =========================================================
if os.getuid() == 0:
    print("Running as root!")
else:
    print("I cannot run as a mortal. Sorry.")
    window = Gtk.Window(title="Chronos Sync")
    window.connect("destroy", lambda w: Gtk.main_quit())
    window.add(Gtk.Label("\nPermission denied!\nYou need root access.\n"))
    window.show_all()

    Gtk.main()
    exit(1)



# UI Elements
# =========================================================

# Building UI from glade's file
builder = Gtk.Builder()
builder.add_from_file("chronos.glade")

# Starting window and set title
window = builder.get_object("window1")
window.set_title("Chronos Sync")
window.show_all()
window.connect("delete-event", Gtk.main_quit)



# Variables
# =========================================================
serverAddress = ""
serverPort    = ""
syncMethod    = ""



# Default Values
# =========================================================

# Set default Server IP Address
serverAddress = builder.get_object("serverAddress")
serverAddress.set_text("127.0.0.1")


# Set default Server Port
serverPort    = builder.get_object("serverPort")
serverPort.set_text("135")


# Set default Synchronization Type
syncTypeModel = Gtk.ListStore(str)          # List of all sync types
syncTypeModel.append(["Basic"])
syncTypeModel.append(["Christian"])
syncTypeModel.append(["NTP"])

syncType = builder.get_object("syncType")   # Get reference of ComboBox
cell = Gtk.CellRendererText()               # Create a Cell structure
syncType.pack_start(cell, 'text')           # Set cells to "text" type
syncType.add_attribute(cell, 'text', 0)     # Create and set first collum to "text" type
syncType.set_model(syncTypeModel)           # Insert list to ComboBox
syncType.set_active(1)                      # Set first element as active option



# Events
# =========================================================



# Test connection
# ================================
def testConnection (out=1):
  hostname = builder.get_object("serverAddress").get_text() # Getting hostname from GUI field
  response = os.system("ping -W 2 -c 1 "+hostname)    # Test ping from bash output
  if response == 0:
    if out==1:
      builder.get_object("out").set_text("Server '"+hostname+"' is up!")
    return 0
  else:
    if out==1:
      builder.get_object("out").set_text("Server '"+hostname+"' is down!")
    return 1
def on_btnTest_clicked (self):
  testConnection ()
builder.get_object("btnTest").connect("clicked", on_btnTest_clicked) # Set click event
  


# Keep Sync NOT_IMPLEMENTED!
# ================================
builder.get_object("btnKeepSync").hide()

# Log NOT_IMPLEMENTED!
# ================================
builder.get_object("btnLog").hide()


# Get Sync Type when changed
# ================================
def on_syncType_changed(self):
  description = builder.get_object("description")
  
  if syncType.get_active() == 0:
      #serverPort.set_text("135 (TPC)")
      serverPort.set_text("1801 (UDP)")
      description.set_text("Just send a single request to server.")

  if syncType.get_active() == 1:
      serverPort.set_text("1801 (UDP)")
      description.set_text("Better for intranet low latency.")

  if syncType.get_active() == 2:
      serverPort.set_text("1801 (UDP)")
      description.set_text("Better for online request with inconstant and high latency.")
syncType.connect("changed", on_syncType_changed)
syncType.set_active(0) # HARD_FIX



# Synchronization
# ================================
def sync_time():
  # OUT message
  builder.get_object("out").set_text("Synchronizing...")


  # Sync Server Request
  # =======================
  def sync_request(a=0,b=0):
    # Test connection
    if testConnection() == 1:
      return 1

    # Get sync method ID to be used
    syncMethod  = str(syncType.get_active())

    # Generating command
    command = str('../dist/client '+str(serverAddress.get_text())+' '+str(syncMethod))

    try:
      # DEBUG command
      print("TRY: "+command)

      # Run command and save log
      instantLog =  subprocess.check_output(command, shell=True)
      os.system(instantLog+" >> server.log")

      # GUI datetime output
      out = builder.get_object("out")
      out.set_text("Date and Time: "+instantLog.replace('\n', ''))
    except Exception, e:
      out = builder.get_object("out").set_text("'"+serverAddress.get_text()+"'is not a time server!")
    


  # Start Sync Thread
  # =======================
  try:
    thread.start_new_thread( sync_request, (1,1) )
  except Exception, errtxt:
    print "ERROR Sync Thread: "
    print errtxt
                            

# Event listiner for btnSyncNow
btnSyncNow = builder.get_object("btnSyncNow")
def on_btnSyncNow_clicked(self):

  # NTP NOT IMPLEMENTED!
  if builder.get_object("syncType").get_active() == 2:
    builder.get_object("out").set_text("NTP not implemented yet!")
    return 1

  sync_time()  
btnSyncNow.connect("clicked", on_btnSyncNow_clicked)





# Start GTK process in loop
Gtk.main()