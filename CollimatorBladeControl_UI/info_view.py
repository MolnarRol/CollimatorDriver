from tkinter import *
from tkinter import ttk
from PIL import ImageTk, Image
import webbrowser


class InfoView:
    INFO_String = ('Collimator blade control interface.\n'
                   'Version: 0.1\n'
                   'Company: ES3\n'
                   'Authors:\n'
                   '\tRoland Molnar (Team leader)\temail: roland.molnar.3@student.tuke.sk\n'
                   '\tBc. Samuel Fertal (Developer)\t email: samuel.fertal@student.tuke.sk\n'
                   '\tBc. Vadym Holysh (Developer)\t email: vadym.holysh@student.tuke.sk')

    def __init__(self, root):
        self.logo = ImageTk.PhotoImage(Image.open("img/logo_big.png"))
        self.github_logo = ImageTk.PhotoImage(Image.open("img/github-mark.png").resize((35, 35)))
        self.doc_icon = ImageTk.PhotoImage(Image.open("img/document-icon-19.png").resize((35, 35)))

        self.window = Toplevel(root)
        self.window.title('Software info')
        self.element = Frame(self.window)
        self.element.pack()
        self.element.columnconfigure(1, weight=1)
        self.element.rowconfigure(1, weight=1)

        img_label = Label(self.element, image=self.logo, text='logo')
        img_label.grid(row=0, column=0, sticky='NEW')

        link_panel = LabelFrame(self.element, text='Useful links')
        link_panel.grid(row=1, column=0, sticky='NSEW')
        link_panel.columnconfigure(0, weight=1)

        source_btn = Button(link_panel, text='Source code', image=self.github_logo, compound=LEFT,
                            command=lambda: webbrowser.open_new(r'https://github.com/MolnarRol/CollimatorDriver'))
        source_btn.grid(row=0, column=0, sticky='EW')

        doc_btn = Button(link_panel, text='Documentation', image=self.doc_icon, compound=LEFT)
        doc_btn.grid(row=1, column=0, sticky='EW')

        info_text = Text(self.element)
        info_text.insert(1.0, self.INFO_String)
        info_text.grid(row=0, column=1, rowspan=2, sticky='NSEW')
        info_text.configure(state='disabled', relief='flat')
