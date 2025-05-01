import java.util.*;
import java.util.regex.*;

public class SQLLexicalAnalyzer {

    // Define token types
    public enum TokenType {
        KEYWORD, IDENTIFIER, OPERATOR, LITERAL, PUNCTUATION, WHITESPACE, COMMENT, DELIMITER, UNKNOWN
    }

    // SQL Keywords (uppercase)
    private static final Set<String> KEYWORDS = new HashSet<>(Arrays.asList(
        "SELECT", "FROM", "WHERE", "AND", "OR", "INSERT", "INTO", "VALUES", "UPDATE", "DELETE",
        "CREATE", "TABLE", "DROP", "ALTER", "JOIN", "ON", "GROUP", "BY", "ORDER", "HAVING", "LIMIT"
    ));

    // Regex pattern for token matching
    private static final Pattern PATTERN = Pattern.compile(
        "(?<KEYWORD>[a-zA-Z_][a-zA-Z0-9_]*)" + "|" +
        "(?<IDENTIFIER>\\[[^\\]]+\\]|[a-zA-Z_][a-zA-Z0-9_]*)" + "|" +
        "(?<OPERATOR>>=|<=|!=|=|<|>)" + "|" +
        "(?<LITERAL>'(?:''|[^'])*'|\\\"(?:\\\\\\\"|[^\"])*\\\"|\\d+(\\.\\d+)?)" + "|" +
        "(?<PUNCTUATION>[(),])" + "|" +
        "(?<COMMENT>--[^\\n]*|/\\*.*?\\*/)" + "|" +
        "(?<DELIMITER>@@|\\$\\$|;)" + "|" +
        "(?<WHITESPACE>\\s+)", Pattern.DOTALL
    );

    // Token class
    public static class Token {
        public final TokenType type;
        public final String value;

        public Token(TokenType type, String value) {
            this.type = type;
            this.value = value;
        }

        @Override
        public String toString() {
            return String.format("Token(%-10s, '%s')", type, value);
        }
    }

    // Tokenizer method
    public static List<Token> tokenize(String sql) {
        List<Token> tokens = new ArrayList<>();
        Matcher matcher = PATTERN.matcher(sql);

        int lastMatchEnd = 0;

        while (matcher.find()) {
            if (matcher.start() > lastMatchEnd) {
                tokens.add(new Token(TokenType.UNKNOWN, sql.substring(lastMatchEnd, matcher.start())));
            }

            String match = matcher.group();
            if (matcher.group("KEYWORD") != null) {
                String upper = match.toUpperCase();
                if (KEYWORDS.contains(upper)) {
                    tokens.add(new Token(TokenType.KEYWORD, match));
                } else {
                    tokens.add(new Token(TokenType.IDENTIFIER, match));
                }
            } else if (matcher.group("IDENTIFIER") != null) {
                tokens.add(new Token(TokenType.IDENTIFIER, match));
            } else if (matcher.group("OPERATOR") != null) {
                tokens.add(new Token(TokenType.OPERATOR, match));
            } else if (matcher.group("LITERAL") != null) {
                tokens.add(new Token(TokenType.LITERAL, match));
            } else if (matcher.group("PUNCTUATION") != null) {
                tokens.add(new Token(TokenType.PUNCTUATION, match));
            } else if (matcher.group("COMMENT") != null) {
                tokens.add(new Token(TokenType.COMMENT, match));
            } else if (matcher.group("DELIMITER") != null) {
                tokens.add(new Token(TokenType.DELIMITER, match));
            } else if (matcher.group("WHITESPACE") != null) {
                tokens.add(new Token(TokenType.WHITESPACE, match));
            }

            lastMatchEnd = matcher.end();
        }

        if (lastMatchEnd < sql.length()) {
            tokens.add(new Token(TokenType.UNKNOWN, sql.substring(lastMatchEnd)));
        }

        return tokens;
    }

    // Main method for testing
    public static void main(String[] args) {
        String sql = "SELECT name, age FROM users WHERE age >= 18 AND name = 'John';";

        List<Token> tokens = tokenize(sql);

        System.out.printf("%-12s | %s%n", "TokenType", "Value");
        System.out.println("-------------|------------------------------");
        for (Token token : tokens) {
            System.out.printf("%-12s | %s%n", token.type, token.value);
        }
    }
}
