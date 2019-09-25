#ifndef _ISO_DATABASE_HPP
#define _ISO_DATABASE_HPP

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <sqlite3.h> 

namespace iso
{
    class Database
    {
        public:
            Database(std::string file)
            {
                int rc = sqlite3_open(file.c_str(), &m_db);

                if( rc != SQLITE_OK ) {
                    m_connected = GL_FALSE;
                    std::cerr << "Can't open database: " << sqlite3_errmsg(m_db) << std::endl;
                } else {
                    m_connected = GL_TRUE;
                    std::cout << "Opened database successfully" << std::endl;
                }
   
            }


            Model get(std::string p_table, GLuint p_id)
            {
                std::string query = "SELECT * FROM " + p_table + " WHERE " + p_table + "_id" + " = " + std::to_string(p_id);
            }

            ~Database()
            {
                sqlite3_close(m_db);
            }



        private:
            sqlite3 *m_db;
            GLboolean m_connected;
    };

    class Model
    {
    public:
        void select_callback(void *data, int p_count, char **p_values, char **p_columns)
        {
            for(GLuint i = 0; i < p_count; i++){
                std::cout << std::string(p_columns[i]) + " = " + std::string(p_values[i] ? p_values[i] : "NULL");
            }
        }

    private:

    };

    class Character : Model
    {
    public:
        Character(GLushort p_character_id, std::string p_name, std::string p_name, GLushort p_model_id)
        {
            m_character_id = p_character_id;
            m_name = p_name;
            m_model_id = p_model_id;
        }

        void select_callback(void *data, int p_count, char **p_values, char **p_columns)
        {
            for(GLuint i = 0; i < p_count; i++){
                std::cout << std::string(p_columns[i]) + " = " + std::string(p_values[i] ? p_values[i] : "NULL");
            }

            Character(std::atoi(p_values[0]), std::string(p_values[1]), std::string(p_values[2]), std::atoi(p_values[3]));
        }

    private:
        std::string m_table = "character";

        GLushort m_character_id;
        std::string m_name;
        GLushort m_model_id;
    };
};

#endif