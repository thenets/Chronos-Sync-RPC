#!/bin/python
from gi.repository import Gtk

import sys
import os
import subprocess



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


# Set default Syncronization Type
syncTypeModel = Gtk.ListStore(str)          # List of all sync types
syncTypeModel.append(["Basic"])
syncTypeModel.append(["Lamport"])
syncTypeModel.append(["Berkeley"])

syncType = builder.get_object("syncType")   # Get reference of ComboBox
cell = Gtk.CellRendererText()               # Create a Cell structure
syncType.pack_start(cell, 'text')           # Set cells to "text" type
syncType.add_attribute(cell, 'text', 0)     # Create and set first collum to "text" type
syncType.set_model(syncTypeModel)           # Insert list to ComboBox
syncType.set_active(1)                      # Set first element as active option



# Events
# =========================================================

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
      description.set_text("Better for intranet with constant latency.")

  if syncType.get_active() == 2:
      serverPort.set_text("1801 (UDP)")
      description.set_text("Better for online request with inconstant latency.")
syncType.connect("changed", on_syncType_changed)
syncType.set_active(0) # HARD_FIX



# Syncronization
# ================================
def sync_time():
  # Get sync method ID to be used
  syncMethod  = str(syncType.get_active())

  # Generating command
  command = str('../dist/client '+str(serverAddress.get_text())+' '+str(syncMethod))

  # DEBUG command
  print("TRY: "+command)

  # Run command and save log
  instantLog =  subprocess.check_output(command, shell=True)
  os.system(instantLog+" >> server.log")

  # GUI datetime output
  out = builder.get_object("out")
  out.set_text("Date and Time: "+instantLog.replace('\n', ''))                          

# Event listiner for btnSyncNow
btnSyncNow = builder.get_object("btnSyncNow")
def on_btnSyncNow_clicked(self):
  sync_time()
btnSyncNow.connect("clicked", on_btnSyncNow_clicked)







# Test connection
# ================================
def check_ping():
    hostname = "taylor"
    response = os.system("ping -c 1 " + hostname)
    # and then check the response...
    if response == 0:
        pingstatus = "Network Active"
    else:
        pingstatus = "Network Error"

    return pingstatus





# Start GTK process in loop
Gtk.main()