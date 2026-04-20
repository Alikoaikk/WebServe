/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imports.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msafa <msafa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:08:49 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/20 00:00:00 by msafa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMPORTS_HPP
#define IMPORTS_HPP

// C++ standard library
# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include <vector>
# include <cstdlib>
# include <cstring>
# include <stdexcept>
# include <ctime>

// POSIX / system headers
# include <unistd.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <poll.h>

// Project class headers
# include "../classes/parsing.hpp"
# include "../classes/Socket.hpp"
# include "../classes/Request.hpp"
# include "../classes/Response.hpp"
# include "../classes/Client.hpp"

#endif
