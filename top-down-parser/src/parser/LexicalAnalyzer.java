package parser;

import java.io.IOException;
import java.io.InputStream;
import java.text.ParseException;

class LexicalAnalyzer {
    private InputStream inputStream;
    private int curChar;
    private int curPos;
    private Token curToken;

    LexicalAnalyzer(InputStream is) throws ParseException {
        inputStream = is;
        curPos = 0;
        nextChar();
    }

    private Boolean isBlank(int c) {
        return Character.isWhitespace(c);
    }

    private void nextChar() throws ParseException {
        curPos++;
        try {
            curChar = inputStream.read();
        } catch (IOException e) {
            throw new ParseException(e.getMessage(), curPos);
        }
    }

    void nextToken() throws ParseException {
        while (isBlank(curChar)) {
            nextChar();
        }

        switch (curChar) {
            case ',':
                nextChar();
                curToken = Token.COMMA;
                break;
            case ':':
                nextChar();
                curToken = Token.TWOSPOT;
                break;
            case ';':
                nextChar();
                curToken = Token.SEMI;
                break;
            case -1:
                curToken = Token.END;
                break;
            default:
                if (!Character.isLetter(curChar)) {
                    throw new ParseException("Illegal character " + (char) curChar, curPos);
                }
                int i = 0;
                int[] chars = new int[3];
                while (Character.isLetter(curChar) || Character.isDigit(curChar)) {
                    if (i < 3) {
                        chars[i] = curChar;
                    }
                    nextChar();
                    i++;
                }
                if (i == 3 && (chars[0] == 'v' || chars[0] == 'V') && (chars[1] == 'a' || chars[1] == 'A') && (chars[2] == 'r' || chars[2] == 'R')) {
                    curToken = Token.VAR;
                } else {
                    curToken = Token.NAME;
                }
        }
    }

    Token getCurToken() {
        return curToken;
    }

    int getCurPos() {
        return curPos;
    }
}


