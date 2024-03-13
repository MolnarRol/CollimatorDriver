from tkinter import *
from tkinter import ttk
from info_view import InfoView


class TopMenuBar:
    def __init__(self, root):
        self.root = root
        self.element = Menu(root)
        self.element.add_command(label='Connection settings', command=self.__connection_settings_clicked__)
        self.element.add_command(label='Info', command=self.__info_menu_clicked__)

    def __info_menu_clicked__(self):
        self.info_window = InfoView(self.root)

    def __connection_settings_clicked__(self):
        pass
