import parser.Parser;
import parser.Tree;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.text.ParseException;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        String str = in.nextLine();
        InputStream is = new ByteArrayInputStream(str.getBytes());
        try {
            Parser parser = new Parser();
            Tree tree = parser.parse(is);
            System.out.println();
            System.out.println(tree.visualisation());
        } catch (ParseException e) {
            e.printStackTrace();
        }
    }
}
