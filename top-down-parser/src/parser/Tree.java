package parser;

import java.util.Arrays;
import java.util.List;

public class Tree {
    private String nodeName;
    private List<Tree> children;

    Tree(String nodeName, Tree... children) {
        this.nodeName = nodeName;
        this.children = Arrays.asList(children);
    }

    Tree(String nodeName) {
        this.nodeName = nodeName;
        children = null;
    }

    public String visualisation() {
        return visualisation(0);
    }

    String visualisation(int depth) {
        StringBuilder answer = new StringBuilder();
        for (int i = 0; i < depth * 5; i++) {
            answer.append(" ");
        }
        answer.append("+----").append(nodeName).append("\n");

        if (children != null) {
            for (Tree child: children) {
                answer.append(child.visualisation(depth + 1));
            }
        }
        return answer.toString();
    }
}
