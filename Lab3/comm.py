#!/usr/bin/python

import sys, locale, string
from optparse import OptionParser

class FileCompareObj(object):
    def __init__(self, filepath1, filepath2):
        """
        Arguments
        file1 -- string of path to first file to be compared
        file2 -- string of path to second file to be compared
        """
        # Read Input files, otherwise Raise IOError
        try:
            if filepath1 == "-" and filepath2 == "-":
                print("Error: cannot read from stdin for two files")
                exit()

            if filepath1 == "-":
                file1 = sys.stdin
            else:
                file1 = open(filepath1, 'r')
            self.lines1 = file1.readlines()
            file1.close()

            if filepath2 == "-":
                file2 = sys.stdin
            else:
                file2 = open(filepath2, 'r')
            self.lines2 = file2.readlines()
            file2.close()

        except IOError as e:
            errno = e.errno
            strerror = e.strerror
            print("IOError: error {0}: {1}".format(errno, strerror))
            exit()

        self.column1 = []
        self.column2 = []
        self.column3 = []


    def CompareSorted(self):
        i = j = 0
        len1 = len(self.lines1)
        len2 = len(self.lines2)
        while i < len1 or j < len2:
            line1 = line2 = ""
            if i < len1:
                line1 = self.lines1[i]
            if j < len2:
                line2 = self.lines2[j]

            if line1 == line2:
                self.column3.append(line1)
                self.column1.append("\t")
                self.column2.append("\t")
                i += 1
                j += 1
            elif line1 == "" or (line2 != "" and line1 > line2):
                self.column2.append(line2)
                self.column1.append("\t")
                self.column3.append("")
                j += 1
            elif line2 == "" or (line1 != "" and line1 < line2):
                self.column1.append(line1)
                self.column2.append("")
                self.column3.append("")
                i += 1


    def CompareUnsorted(self):
        i = 0
        while i < len(self.lines1):
            foundMatch = False
            j = 0
            while j < len(self.lines2):
                if self.lines1[i] == self.lines2[j]:
                    self.column3.append(self.lines1[i])
                    self.column1.append("\t")
                    self.column2.append("\t")
                    foundMatch = True
                    self.lines2.pop(j)
                j += 1
            if foundMatch == False:
                self.column1.append(self.lines1[i])
                self.column2.append("")
                self.column3.append("")
            i += 1

        i = j = 0
        for j in range(len(self.lines2)):
            foundMatch = False
            for i in range(len(self.lines1)):
                if self.lines2[j] == self.lines1[i]:
                    foundMatch = True
                    continue
            if foundMatch == False:
                self.column2.append(self.lines2[j])
                self.column1.append("\t")
                self.column3.append("")

    def PrintComparison(self, sup1, sup2, sup3):
        if sup1:
            self.column1 = [""] * len(self.column1)
        if sup2:
            self.column2 = [""] * len(self.column2)
        if sup3:
            self.column3 = [""] * len(self.column3)

        final = []
        for i in range(len(self.column1)):
            final.append(self.column1[i] + self.column2[i] + self.column3[i])
        for i in range(len(final)):
            sys.stdout.write(final[i].rstrip("\t"))
            pass

def main():
    version_msg = "%prog 1.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2

    Compare sorted files FILE1 and FILE2 line by line."""

    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-1", action="store_true", dest="sup_1", 
            help="suppress column 1 (lines unique to FILE1)")
    parser.add_option("-2", action="store_true", dest="sup_2",
            help="suppress column 2 (lines unique to FILE2)")
    parser.add_option("-3", action="store_true", dest="sup_3",
            help="suppress column 3 (lines unique to FILE3)")
    parser.add_option("-u", action="store_true", dest="unsorted", 
            help="allows files to be compared files unsorted")
    options, args = parser.parse_args(sys.argv[1:])

    try:
        sup1 = bool(options.sup_1)
    except:
        parser.error("invalid option -- {0}".format(options.sup_1))
    try:
        sup2 = bool(options.sup_2)
    except:
        parser.error("invalid option -- {0}".format(options.sup_2))
    try:
        sup3 = bool(options.sup_3)
    except:
        parser.error("invalid option -- {0}".format(options.sup_3))
    try:
        unsorted = bool(options.unsorted)
    except:
        parser.error("invalid option -- {0}".format(options.unsorted))

    if len(args) != 2:
        parser.error("missing file arguments")
    filepath1 = args[0]
    filepath2 = args[1]

    cmp = FileCompareObj(filepath1, filepath2)
    if unsorted:
        cmp.CompareUnsorted()
    else:
        cmp.CompareSorted()
    cmp.PrintComparison(sup1, sup2, sup3)

if __name__ == "__main__":
    main()
