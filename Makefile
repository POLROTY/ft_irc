NAME		= ircserv

SRC			= test.cpp \
			  User.cpp \
			  Server.cpp

SRCSPATH 	= srcs

DEPS		= {SRCS:.cpp=.d}

INCLUDES	= ./includes/

OBJDIR		= all_o

CC			= c++

CFLAGS		= -MD -Werror -Wall -Wextra -g3 -std=c++98

RM			= rm -rf

SRCS = $(addprefix $(SRCSPATH)/,$(SRC))

OBJS        = $(addprefix $(OBJDIR)/,${SRC:.cpp=.o})

$(OBJDIR)/%.o: $(SRCSPATH)/%.cpp
	@printf "$(_ORANGE)Compiling : %s\n" $<
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I $(INCLUDES) -c $< -o $@

all: ${NAME}

${NAME}:	${OBJS}
	@${CC} ${CFLAGS} -I $(INCLUDES) ${OBJS} -o ${NAME}
	@printf "$(_GREEN)Compiling DONE !\n"
	@printf "$(_CYAN)${BANNERPROJECT}\n"

clean:
	@printf "$(_RED)Removing objects...\n"
	@${RM} ${OBJS} ./all_o/*.o ./all_o/*.d

fclean:	clean
	@printf "$(_RED)Removing $(NAME)...\n"
	@${RM} ${NAME} ./all_o/*.o ./all_o/*. all_o

re:	fclean all

-include $(DEPS)

################################################# colors

_WHITE = \e[97m
_CYAN = \033[5;36m
_GREEN = \e[38;5;118m
_ORANGE = \e[38;5;208m
_RED = \e[38;5;196m
_R = \e[0m

#################################################

.PHONY: all clean fclean re

################################################# banner

BANNERPROJECT :=\
\n\
 ██████╗ ██████╗ ███╗   ███╗███████╗  ████████╗ ██████╗   ██╗██████╗  ██████╗ \n\
██╔════╝██╔═══██╗████╗ ████║██╔════╝  ╚══██╔══╝██╔═══██╗  ██║██╔══██╗██╔════╝ \n\
██║     ██║   ██║██╔████╔██║█████╗       ██║   ██║   ██║  ██║██████╔╝██║      \n\
██║     ██║   ██║██║╚██╔╝██║██╔══╝       ██║   ██║   ██║  ██║██╔══██╗██║      \n\
╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗     ██║   ╚██████╔╝  ██║██║  ██║╚██████╗ \n\
 ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝     ╚═╝    ╚═════╝   ╚═╝╚═╝  ╚═╝ ╚═════╝ \n\
\n\