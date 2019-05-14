package parser;

import java.io.InputStream;
import java.text.ParseException;

import static parser.Token.*;

public class Parser {
    private LexicalAnalyzer lexicalAnalyzer;

    private Tree S() throws ParseException {
        switch (lexicalAnalyzer.getCurToken()) {
            case VAR:
                lexicalAnalyzer.nextToken();
                Tree child = SPrime();
                return new Tree("S", new Tree("VAR"), child);
            default:
                throw new AssertionError();
        }
    }

    private Tree SPrime() throws ParseException {
        switch (lexicalAnalyzer.getCurToken()) {
            case NAME:
                Tree child1 = E();
                Tree child2 = S2Prime();
                return new Tree("SPrime", child1, child2);
            default:
                throw new AssertionError();
        }
    }

    Tree E() throws ParseException {
        switch (lexicalAnalyzer.getCurToken()) {
            case NAME:
                Tree child1 = V();
                if (lexicalAnalyzer.getCurToken() != TWOSPOT) {
                    throw new ParseException(": expected at position", lexicalAnalyzer.getCurPos());
                }
                lexicalAnalyzer.nextToken();
                Tree child2 = T();
                if (lexicalAnalyzer.getCurToken() != SEMI) {
                    throw new ParseException("; expected at position", lexicalAnalyzer.getCurPos());
                }
                lexicalAnalyzer.nextToken();
                return new Tree("E", child1, new Tree("TWOSPOT"), child2, new Tree("SEMI"));
            default:
                throw new AssertionError();
        }
    }

    private Tree S2Prime() throws ParseException {
        switch (lexicalAnalyzer.getCurToken()) {
            case NAME:
                Tree child1 = E();
                Tree child2 = S2Prime();
                return new Tree("S2Prime", child1, child2);
            case END:
                return new Tree("S2Prime", new Tree("EPS"));
            default:
                throw new AssertionError();
        }
    }

    private Tree V() throws ParseException {
        switch (lexicalAnalyzer.getCurToken()) {
            case NAME:
                lexicalAnalyzer.nextToken();
                Tree child = VPrime();
                return new Tree("V", new Tree("NAME"), child);
            default:
                throw new AssertionError();
        }
    }

    private Tree T() throws ParseException {
        switch (lexicalAnalyzer.getCurToken()) {
            case NAME:
                lexicalAnalyzer.nextToken();
                return new Tree("T", new Tree("NAME"));
            default:
                throw new AssertionError();
        }
    }

    private Tree VPrime() throws ParseException {
        switch (lexicalAnalyzer.getCurToken()) {
            case COMMA:
                lexicalAnalyzer.nextToken();
                if (lexicalAnalyzer.getCurToken() != NAME) {
                    throw new ParseException("NAME expected at position", lexicalAnalyzer.getCurPos());
                }
                lexicalAnalyzer.nextToken();
                Tree child = VPrime();
                return new Tree("VPrime", new Tree("NAME"), child);
            case TWOSPOT:
                return new Tree("VPrime", new Tree("EPS"));
            default:
                throw new AssertionError();
        }
    }

    public Tree parse(InputStream inputStream) throws ParseException {
        lexicalAnalyzer = new LexicalAnalyzer(inputStream);
        lexicalAnalyzer.nextToken();
        return S();
    }

}
