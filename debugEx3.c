#include <stdio.h>
#include <stdlib.h>

struct url_t {
	char protocol[100];
	char hostname[256];
	char path[1024];
	char queryString[1024];
};

enum States {
	Start,
	Protocol,
	Colon,
	Slash1,
	Slash2,
	Host,
	Path,
	QueryString,
	Failed,
	Stop
};

int is_alpha(char ch) {
	return  ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
			|| (ch == '-') || (ch == '_') || (ch == '.'));
}

int is_digit(char ch) {
	return (ch >= '0' && ch <= '9');
}
int is_hostchar(char ch) {
	return is_alpha(ch) || is_digit(ch);
}

int is_pathchar(char ch) {
	return is_alpha(ch) || is_digit(ch) || (ch == '/');
}

int is_querychar(char ch) {
	return is_alpha(ch) || is_digit(ch) || (ch == '?') || (ch == '&');
}


int transition(int state, char ch) {
	switch (state) {
		case Start:
			if (is_alpha(ch)) state = Protocol;
			else state = Failed;
		break;

		case Protocol:
			if (is_alpha(ch)) state = Protocol;
			else if (ch == ':') state = Colon;
			else state = Failed;
		break;

		case Colon: state = (ch == '/') ? Slash1 : Failed;
		
		case Slash1: state = (ch == '/') ? Slash2 : Failed;
		break;

		case Slash2: state = is_pathchar(ch) ? Host : Failed;
		break;

		case Host: state = is_hostchar(ch) ? Host : ((ch == '/') ? Path : Failed);
		break;

		case Path: state = is_pathchar(ch) ? Path : ((ch == '?') ? QueryString: Stop);
		break;
		
		case QueryString: state = is_querychar(ch) ? QueryString : Stop;
		break;

		default: 
		;
	}
	return state;
}


void read_line(char* str, int maxLen) {
	int ch = 0, ctr = 0;
	while ((ctr < maxLen) && ((ch = getchar()) != '\n')) {
		str[ctr++] = ch;
	}
	str[ctr] = 0;
}
void print_url(struct url_t* u) {
	printf("Protocol: %s\n", u->protocol);
	printf("Host: %s\n", u->hostname);
	printf("Path: %s\n", u->path);
	printf("Query: %s\n", u->queryString);
}


int main() {
	char str[1024];
	struct url_t url;
	int state = Start, prevState = Start;
	int i = 0;
	read_line(str, 1024);
	char* ptr = str;
	for (; *ptr != '\0'; ++ptr) {
		state = transition(prevState, *ptr);
		if (state == Protocol) {
			url.protocol[i++] = *ptr;
		} else if (state == Host) {
			if (prevState == Slash2) {
				url.protocol[i] = 0;
				i = 0;
			}
			url.hostname[i++] = *ptr;
		} else if (state == Path) {
			if (prevState == Host) {
				url.hostname[i] = 0;
				i = 0;
			}
			url.path[i++] = *ptr;
		} else if (state == QueryString) {
			if (prevState == Path) {
				url.path[i] = 0;
				i = 0;
			}
			url.queryString[i++] = *ptr;
		} else if (state == Stop) {
			url.queryString[i] = 0;
		}
		prevState = state;
	}
	url.queryString[i] = 0;

	print_url(&url);
	return 0;

}
