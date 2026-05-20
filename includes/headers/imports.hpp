/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   imports.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akoaik <akoaik@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 17:08:49 by akoaik            #+#    #+#             */
/*   Updated: 2026/04/20 15:38:25 by akoaik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IMPORTS_HPP
#define IMPORTS_HPP

// C++ standard library
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// system headers
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>

// Project class headers
#include "../classes/Client.hpp"
#include "../classes/EventLoop.hpp"
#include "../classes/Parsing.hpp"
#include "../classes/Request.hpp"
#include "../classes/Response.hpp"
#include "../classes/Server.hpp"

#endif
